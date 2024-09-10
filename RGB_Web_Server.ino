// Ryan Root
// 8/21/24
// ESP32 Network RGB Strip Controller

// This will use the ledc library for PWM control, which is included in arduino core
// Ardunio core for the ESP32 can be found here: https://github.com/espressif/arduino-esp32

#include <WiFi.h>

const char* ssid = "";           // your network SSID
const char* password = "";       // your network password

// GPIO pins for RGB strip
const int green_pin = 16;
const int blue_pin = 17;
const int red_pin = 18;

// PWM control setup
const int frequency = 500;
const int resolution = 8;

// set port number 80
WiFiServer server(80);

// string to store the HTTP request
String header;

// variables to handle page timeout
unsigned long current_time = millis();
unsigned long prev_time = 0;
const long timeout_period = 5000;   // 5 seconds

// change RGB colors given parsed values
// red, green, blue must be ints between 0 and 255
void change_color(int red, int green, int blue) {
  ledcWrite(red_pin, red);
  ledcWrite(green_pin, green);
  ledcWrite(blue_pin, blue);
}

// connect to the network with given SSID and password, start server
void initwifi() {
  WiFi.begin(ssid, password);
  Serial.print("ESP is connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("...");
    delay(500);
  }
  Serial.print("Connected. Current IP: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void setup() {
  // highest BAUD rate
  Serial.begin(115200);

  ledcAttach(green_pin, frequency, resolution);
  ledcAttach(blue_pin, frequency, resolution);
  ledcAttach(red_pin, frequency, resolution);

  initwifi();
}

void loop() {
  // Listen for incoming clients
  WiFiClient client = server.available();

  if (client) {
    current_time = millis();
    prev_time = current_time;
    Serial.println("A client has connected.");
    String curr_line = "";      // a String to hold server data
    while (client.connected() && current_time - prev_time <= timeout_period) {
      current_time = millis();
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;

        if (c == '\n') {
          if (curr_line.length() == 0) {
            // HTTP header
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // display web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta charset=\"UTF-8\"><meta name=\"width=device-width, initial-scale=1.0\">");
            client.println("<h1>Control Your RGB Strip</h1>");
            // CSS styling
            client.println("<style>h1 {color: black; text-align: center;}");
            client.println("h2 {text-align: center;}");
            client.println("button {font-size: larger; border-radius: 12px; background-color: gainsboro; padding: 10px 15px;}");
            client.println("form {text-align: center; font-size: medium;}</style></head>");
            client.println("<body><h2>Enter a value between 0 and 255 for each of the following colors:</h2>");
            // JS script to update query string
            client.println("<script> function get_color() {;");
            client.println("var red_val = document.getElementById(\"red\").value;");
            client.println("var green_val = document.getElementById(\"green\").value;");
            client.println("var blue_val = document.getElementById(\"blue\").value;");
            client.println("const obj = {r: red_val, g: green_val, b: blue_val,};");
            client.println("const search_params = new URLSearchParams(obj);");
            client.println("const query_string = search_params.toString();");
            client.println("const new_url = window.location.origin + '/?' + query_string + \"END\";");
            client.println("window.location.href = new_url; } </script>");
            // form for user to input RGB values
            client.println("<form>Red:<input type = \"text\" size = \"12\" id = \"red\">");
            client.println("Green:<input type = \"text\" size = \"12\" id = \"green\">");
            client.println("Blue:<input type = \"text\" size = \"12\" id = \"blue\">");
            client.println("<button type=\"button\" onclick=\"get_color()\">Apply</button></form>");
            client.println();

            // grab RGB values that were submitted
            if (header.indexOf("GET /?r=") >= 0) {
              int index_1 = header.indexOf("r=");
              int index_2 = header.indexOf("g=");
              int index_3 = header.indexOf("b=");
              int index_4 = header.indexOf("END");

              String red_string = header.substring(index_1 + 2, index_2 - 1);
              String green_string = header.substring(index_2 + 2, index_3 - 1);
              String blue_string = header.substring(index_3 + 2, index_4);

              int red_val = red_string.toInt();
              int green_val = green_string.toInt();
              int blue_val = blue_string.toInt();
              change_color(red_val, green_val, blue_val);
            }
            break;
          } else {
            curr_line = "";
          }
        } else if (c != '\r') {
          curr_line += c;
        }
      }
    }
    // reset header
    header = "";
    // close connection
    client.stop();
    Serial.println("Disconnected.");
    Serial.println();
  }
}
