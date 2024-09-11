# ESP32-Wireless-RGB-Strip-Control
Use an ESP32 microcontroller to wirelessly connect to and control an RGB LED strip on your local network.
![](https://github.com/user-attachments/assets/2b270d22-698a-43ce-8e5d-67e09a277b6b)![](https://github.com/user-attachments/assets/8f7f4a08-d5a6-4a3b-8b12-5115b76e43d3)



## **Materials:**
There are a handful of materials needed for this project, all of which can be found for purchase at various places online. This includes an **ESP32 Microcontroller**, and basic prototyping equipment such as a **breadboard** and **jumper wires**. Also needed will be an **RGB LED Strip**. The one I chose for this project is a 5V, 4-pin strip with a common cathode. Some type of **power delivery transistor/mosfet** will be needed as well, which should be chosen based on your power needs. More on this in the Circuit Design section.


## **Circuit Design:**
The wiring for this project is relatively simple, and a simple pictorial made in TinkerCad can be seen below for reference. 

Three different GPIO pins will be connected to the base of the transistors, while the emitter should be connected to ground, and the collector to the RGB strips color pin. Ensure that the transistors chosen for the project can handle the current you will be passing through them. I chose to use three [S8050](https://components101.com/transistors/s8050-transistor-pinout-equivalent-datasheet) NPN-type transistors, which have an EBC pinout. 

If using a 5V RGB strip, it can be powered directly from the VIN pin on the ESP32. This assumes that you are powering the ESP32 through the 5V USB type C (sometimes micro USB) connection on the front of the board. If the strip used requires more than 5V, or the board cannot provide succficent power, and external power source may be used. If using an external power source ensure that there is a common ground shared between the board and the power supply.

***Use extra caution when using the VIN pin, as it has NO reverse polarity protection.***


![Wiring Pictorial](https://github.com/user-attachments/assets/9004e138-8654-42f3-8b6a-8fd9b7ed9241)


***Note:*** Notice an electrolytic capacitor in the diagram which was not mentioned. It is common to get a "Failed to connect to ESP32" error when uploading code to your board. This is because the ESP32 will not go into uploading mode automatically when code is uploaded to it. This can be fixed by pressing the BOOT button on the board when uploading code. You can also attach a 10uF capacitor to the ground and EN pins on the board. Doing this allows code to be uploaded without pressing the BOOT button.



## **Code:**
I used the Arduino IDE to write and compile/upload code to my ESP32 for this project. This requires some setup in the Arduino IDE, if you have not used the Arduino IDE with an ESP32 before, you will need Arduino Core for the ESP32 installed. Follow this [installation guide](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html) from Espressif.

For PWM control over the RGB LED strip, we will be using the ledc library. This is included in Arduino Core, and the documentation for it can be found [here](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/ledc.html#arduino-esp32-ledc-api). An example of strictly PWM control can be found at [Working Examples/RGB_PWM](Working%20Examples/RGB_PWM.ino).

For wifi connections, we will use the WiFi Arduino library, which can be installed directly from the library manager in the Arduino IDE. Documentation for this library can be found [here](https://www.arduino.cc/reference/en/libraries/wifi/).

The web page itself is an HTML file that uses CSS for styling, and short Javascript script. It will display a form that allows users to input values between 0 and 255 for each RGB value. These values are then appended to a query string on the URL, and grabbed by the ESP32 to make changes to the RGB strip. The HTML page can be found at [Working Examples/index](Working%20Examples/index.html).
