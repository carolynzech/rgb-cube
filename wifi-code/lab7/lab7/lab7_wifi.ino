#include <Arduino_JSON.h>
#include <assert.h>

char ssid[] = "Brown-Guest";  // network SSID (name)
char pass[] = ""; // for networks that require a password
int status = WL_IDLE_STATUS;  // the WiFi radio's status
char color;
char* api_url = "https://api.weather.gov/gridpoints/BOX/64,64/forecast"; // default location is Brown


void setup_wifi() {
  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid); // WiFi.begin(ssid, pass) for password
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
  if (client.connect("api.weather.gov", 80)) {
    client.println("GET /gridpoints/BOX/64,64/forecast");
    client.println("Host: api.weather.gov");
    client.println("Connection: close");
    client.println();
    return true;
  } else {
    Serial.println("Failed to fetch webpage");
    return false;
  }
}

void read_webpage() {
    // Check for bytes to read
  int len = client.available();
  if (len == 0){
    return;
  }
  memset(buffer, 0x0, sizeof(buffer));
  int index = 0;
  while (client.available()) {
    char c = client.read();
    buffer[index] = c;
    index++;
    // Serial.println(c);

  //  JSONVar myObject = JSON.parse(response);

    char* red = strstr(buffer, "shortForecast");
    char* blue = strstr(buffer, "BLUE");
    char* green = strstr(buffer, "GREEN");

    if (green != NULL) {
      color = 'G';
    } else if (red != NULL) {
      color = 'R';
      Serial.println("found short forecast");
    } else if (blue != NULL) {
      color = 'B';
    }
  }
  
  /*
   * LAB STEP 3b: change above code to only print RED, GREEN, or BLUE as it is read
   * LAB STEP 3c: change above code to send 'r', 'g', or 'b' via Serial1
   * LAB STEP 4e: change above code to put 'r', 'b', or 'g' in s_buf for sending on UART
   */
}
