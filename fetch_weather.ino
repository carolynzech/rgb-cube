#include <SPI.h>
#include <WiFi101.h>
#include <assert.h>

char ssid[] = "Brown-Guest";  // network SSID (name)
char pass[] = ""; // for networks that require a password
int status = WL_IDLE_STATUS;  // the WiFi radio's status
WiFiClient client;
char buffer[2000];
const int buf_size = 2000;

void setup_wifi() {
  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid);
    // status = WiFi.begin(ssid, pass)
    delay(10000);
  }
  Serial.println("Connected!");
  if (connect_to_webpage()) {
    Serial.println("fetched webpage");
  } else {
    Serial.println("failed to fetch webpage");
  }
  Serial.println();
}

bool connect_to_webpage() {
  if (client.connect("api.open-meteo.com", 80)) { // (client.connect(api.weather.gov, 80))
    client.println("GET /v1/forecast?latitude=52.52&longitude=13.41&current_weather=true HTTP/1.1"); // gridpoints/BOX/64,64/forecast?units=us
    client.println("Host: api.open-meteo.com");
    client.println("Connection: close");
    client.println();
    return true;
  } else {
    Serial.println("Failed to fetch webpage");
    return false;
  }
}

int read_webpage() {
    // Check for bytes to read
  int len = client.available();
  if (len == 0){
    // Serial.println("returning");
    return -1;
  }
  memset(buffer, 0x0, sizeof(buffer));
  int index = 0;
  while (client.available()) {
    int r = client.read();
    char c = (char) r;
    buffer[index] = c;
    index++;
    char* time_ptr = strstr(buffer, "time\":");
    char weather_code_digit_one;
    char weather_code_digit_two;
    int weather_code = -2;
    if (time_ptr != NULL) {
      weather_code_digit_one = *(time_ptr - 4);
      weather_code_digit_two = *(time_ptr - 3);
      if (weather_code_digit_one == ':') { // weather code is one digit
        weather_code = int(weather_code_digit_two);
      } else { // weather code is two digits
        char arr[3];
        arr[0] = weather_code_digit_one;
        arr[1] = weather_code_digit_two;
        arr[2] = '\0';
        weather_code = int(arr);
      }
      return weather_code;
    }
  }
  return -1;
}
   
void check_connection() {
    // Connection ended / no more bytes to read
    if (!client.connected()) {
      delay(500);
      // Try to reconnect
      connect_to_webpage();
    }
}
