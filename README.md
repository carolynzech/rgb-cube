# rgb-cube
## General Description 
Our project is a 4x4x4 RGB LED Cube that queries a weather API and changes the display depending on the weather state. The programmed states that it responds to are clear skies, cloudy, snowy, and rainy. If the state is not any of those four, the cube turns the lights off. We are assuming that the user doesn’t need precise weather states—for example, our cube will display the same rainy pattern regardless of whether it is drizzling or storming. We are also assuming that the user knows how to upload code to an Arduino.

## Description of each file
#### animations.ino
This file controls the display of the LEDs on the cube, in terms of which LEDs are driven, what color they are, and their brightness. It defines multiple different patterns, most of which are driven by charlieplexing using the Arduino’s registers and one that is controlled using the Arduino’s analog write function.
This file satisfies the PWM requirement in lines 434-441.

#### fetch_weather.ino
This file connects the Arduino to WiFi and polls the weather API via WiFi.
This file satisfies the WiFi requirement in lines 1-105.

#### rgb-cube.ino
This file contains the interrupt service routine that polls the weather API. The state in our FSM is updated using the data polled from the API, depending on the current weather in the requested location and lights up the cube accordingly using the patterns from animations.ino. This file also sets up a watchdog timer to ensure that the program will not hang when polling data.
This file satisfies the watchdog timer in lines 90-112, 238-244, and 277-279 as well as the interrupt service routine found in lines 76-88, 118-133, and 223-236.

#### tests.ino
This file contains our unit tests, a more in depth explanation of them can be found in section 7.

## How to run
Pull the code and upload it to an Arduino MKR1000.

## How to run tests
Uncomment the #define TESTING line in the rgb_cube.ino file.

## Appendix
Table for weather codes from API to weather descriptions
<img width="706" alt="Screen Shot 2022-12-04 at 12 17 01 PM" src="https://user-images.githubusercontent.com/60265028/205505447-174facbd-1e0c-47ef-bc61-1a24316a9377.png">
