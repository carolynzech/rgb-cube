/*
 * CSCI 1600 lab 7 starter code
 * Author: Milda Zizyte
 */

#include <SPI.h>
#include <WiFi101.h>

WiFiClient client;
char buffer[20000];

const int buf_size = 80;


/*
 * LAB STEP 4c/g
 * Change this to false for receiver!!
 */
const bool SENDER = true;

void setup() {
  //Initialize serial and wait for port to open:
  // Serial.begin(9600);
  while (!Serial);
  Serial1.begin(9600);

 
  
  if (SENDER) { setup_wifi(); }
}

void loop() {
  if (SENDER) {
    read_webpage();


    // Connection ended / no more bytes to read
    if (!client.connected()) {
      delay(500);
      // Try to reconnect
      connect_to_webpage();
    }
  }
}
