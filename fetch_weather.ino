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

/* 
 * Attempts to connect to the weather API and get the weather for specified location
 * Returns boolean: true for successful connection, false otherwise
 */
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

/*
 * Parses the response from the weather API. Desired information is in form "weathercode: (int)", 
 * where the integer maps to a weather state (see lists in rgb-cube.ino).
 * Input: pointer to "time". The string comes back in form 
 * {... "weathercode":__,"time":"..."}, where weathercode is a one or two digit integer.
 * Returns int: weather code returned from API.
*/
int get_weather_from_time(char* time_ptr) {
  if (time_ptr == NULL) return -1;
  int weather_code;
  // weather code is 3/4 digits back from "time", depending on # of digits
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
  
  if ((weather_code == 0) && (weather_code_digit_two[0] != '0')) return -1;
  return weather_code;
}

/*
 * Reads response from the weather API
 * Returns int: -1 if client unavailable, and weather code otherwise
*/
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
    // "time:" comes immediately after weathercode, so if it's not null, we are ready to parse the weather code
    if (time_ptr != NULL) {
      return get_weather_from_time(time_ptr);
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
