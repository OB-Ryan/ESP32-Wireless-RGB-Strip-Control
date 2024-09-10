// Ryan Root
// 8/21/24
// getting PWM control for RGB strip working

const int green_pin = 16;
const int blue_pin = 17;
const int red_pin = 18;


// PWM Control Setup
const int frequency = 5000;
const int resolution = 8;

void change_color(int red, int green, int blue) {
  ledcWrite(red_pin, red);
  ledcWrite(green_pin, green);
  ledcWrite(blue_pin, blue);
}

// the updated ledc API will automaticall assign pins to PWM channels
// use ledcAttach(uint8_t pin, uint32_t freq, uint8_t resolution);
void setup() {
  ledcAttach(green_pin, frequency, resolution);
  ledcAttach(blue_pin, frequency, resolution);
  ledcAttach(red_pin, frequency, resolution);
}

void loop() {
  for (int i = 0; i <= 255; i++) {
    change_color(i, i, i);
    delay(5);
  }
}
