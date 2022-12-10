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
}

bool connect_to_webpage() {
  if (client.connect("api.open-meteo.com", 80)) {
    client.println("GET /v1/forecast?latitude=41.82&longitude=-71.41&current_weather=true HTTP/1.1"); // providence
    // client.println("GET /v1/forecast?latitude=48.13&longitude=11.58&current_weather=true HTTP/1.1"); // munich
    client.println("Host: api.open-meteo.com");
    client.println("Connection: close");
    client.println();
    return true;
  } else {
    Serial.println("Failed to fetch webpage");
    return false;
  }
}

int get_weather_from_time(char* time_ptr) {
  int weather_code = -1;
  if (time_ptr != NULL) {
    const char* weather_code_digit_one = time_ptr - 4;
    const char* weather_code_digit_two = time_ptr - 3;
    
    if (*weather_code_digit_one == ':') { // weather code is one digit
      weather_code = std::atoi(weather_code_digit_two);
    } else { // weather code is two digits
      char arr[3];
      arr[0] = *weather_code_digit_one;
      arr[1] = *weather_code_digit_two;
      arr[2] = '\0';
      const char* const_arr = arr;
      weather_code = std::atoi(const_arr);
    }
  }
  return weather_code;
}

// -1 error means client unavailable, -2 means just haven't read enough bytes to reach weather code yet
int read_webpage() {
    // Check for bytes to read
  int len = client.available();
  if (len == 0){
    return -1;
  }
  memset(buffer, 0x0, sizeof(buffer));
  int index = 0;
  while (client.available()) {
    char c = client.read();
    buffer[index] = c;
    index++;
    char* time_ptr = strstr(buffer, "time\":");
    int weather_code = get_weather_from_time(time_ptr);
    if (weather_code > 0) return weather_code;
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
