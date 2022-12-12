#include <SPI.h>
#include <WiFi101.h>
#include <assert.h>

char ssid[] = "Brown-Guest";  // network SSID (name)
char pass[] = ""; // for networks that require a password
int status = WL_IDLE_STATUS;  // the WiFi radio's status
WiFiClient client;
char buffer[2000];
const int buf_size = 2000;
char* lat_long_response;

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
}

/* 
 * Attempts to connect to the weather API and get the weather for specified location
 * Returns boolean: true for successful connection, false otherwise
 */
bool connect_to_webpage(char* get_string) {
  Serial.println("Currently polling...");
  Serial.println(get_string);
  if (client.connect("api.open-meteo.com", 80)) {
    client.println(get_string); // providence
    // "GET /v1/forecast?latitude=41.82&longitude=-71.41&current_weather=true HTTP/1.1" providence
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
   
void check_connection(char* get_string) {
    // Connection ended / no more bytes to read
    if (!client.connected()) {
      delay(500);
      // Try to reconnect
      connect_to_webpage(get_string);
    }
}


    // Capstone functions
/* 
 * Parses a portion of the response string from the locations api to find the latitude or longitude within it
 * input: char* start: the starting location from which to look for a decimal number representing either latitude or longitude
 */    
char* trim_str(char* start){
  char *arr = (char *) malloc(10);
  int i = 0;
  int comma = ',';
  int bracket = '}';
  while (i < 10 && (start[i] != comma) && (start[i] != bracket) ){
    arr[i] = *(start + i);
    i++;
  }
  arr[i] = '\0';
  return arr;
}

/* 
 * Attempts to connect to the location API and get the latitude, longitude for a specified user
 * Returns char* : "unavailable" if the client is not available or returns an error or the string that the 
 * connect_to_webpage function should use to make a GET request to the weather API
 */
char* read_location_webpage() {
  char* default_location =  "unavailable";
    // Check for bytes to read
  int len = client.available();
  if (len == 0){
    // Serial.println("returning");
    return default_location; 
  }
  memset(buffer, 0x0, sizeof(buffer));
  int index = 0;
  bool found_lat = false;
  while (client.available()) {
    Serial.println("available");
    int r = client.read();
    char c = (char) r;
    buffer[index] = c;
    index++;
    
    char* start_lat = strstr(buffer, "\"latitude\":");
    char* start_long = strstr(buffer, ",\"longitude");
    char* test_lat;
    if (start_long != NULL && !found_lat){
      found_lat = true;
      char* lat_num = (start_lat + 11);
      test_lat = trim_str(lat_num); 
    }
    char* end_long = strstr(buffer, "}");
    if (start_long != NULL && end_long != NULL){
      char* long_num = (start_long + 13);
      char* test_long = trim_str(long_num);
      char *result = (char *) malloc(100);  // array to hold the result.
      strcpy(result,"GET /v1/forecast?latitude="); // copy string one into the result.
      strcat(result,test_lat);
      strcat(result,"&longitude=");
      strcat(result,test_long);
      strcat(result, "&current_weather=true HTTP/1.1");
      lat_long_response = result;
      return lat_long_response;
    }
  }
  return default_location;
}

bool connect_to_location_webpage() {
  if (client.connect("rgb-led-app.herokuapp.com", 80)) { // (client.connect(api.weather.gov, 80))
    client.println("GET /api/test_string HTTP/1.1"); // gridpoints/BOX/64,64/forecast?units=us
    client.println("Host: rgb-led-app.herokuapp.com");
    client.println("Connection: close");
    client.println();
    return true;
  } else {
    Serial.println("Failed to fetch webpage");
    return false;
  }
}
