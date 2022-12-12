char* mock_response; // so we don't have the memory issue of returning value garbage from a function

/*
 * Compares to char* character by character. A helper function for testing.
 * input: char* a is the first char* to compare
 * input char* b is the first char* to compare
 * output: if a and b are character-by-character equal
 */
bool cmp_char_star(char* a, char* b) {
  int i = 0;
  while ((a[i]) != '\0' && b[i] != '\0'){
    if ((a[i]) != ( b[i])){ // found something that does not match
      return false;
    } else {
    }
    i++;
  }
  if (a[i] == '\0' && b[i] == '\0'){ // were same length
    return true;
  } else { // were not same length
    return false;
  }
}

/*
 * Tests the trim_str function
 * input: char* buff the buffer to trim
 * input: char* expected result the expected result of calling trim_str on the buffer 
 * input: offset used because latitude and longitude have different lengths, 
 * so this depends on which we are testing. this integration tested in mock_read_location_webpage.
 * output: if expected_res equals trim_str of the inputed buffer
 */
bool test_trim(char* buff, char* expected_res, int offset){ 
  char* start = (buff + offset);
  char* res = trim_str(start);
  return cmp_char_star(expected_res, res);
}

/*
 * Tests the mock_read_location function by taking in a mock_buffer and the expected result.
 * input: char* mock_buffer is the input passed to mock_read_location to represent a buffer read in the from the rgb website API
 * input: char* expected_res the expected result
 * output: returns if expected_res matches result of calling mock_read_location on mock_buff
 */
bool test_read_location(char *mock_buff, char* expected_res){
  char* res = mock_read_location_webpage(mock_buff);
  return cmp_char_star(expected_res, res);
}

/* 
 * Version of read location that does not actually read in from a buffer, but instead takes in a buffer to test the string parsing functionality
 * input: char* mock_buffer represents the response that the real function reads in from the API
 * output: returns the API request string created from the latitude and longitude received in the buffer
 */
char* mock_read_location_webpage(char* mock_buffer) {
  char* default_location =  "unavailable";
   bool found_lat = false;
    
    char* start_lat = strstr(mock_buffer, "\"latitude\":");
    char* start_long = strstr(mock_buffer, ",\"longitude");
    char* test_lat;
    if (start_long != NULL && !found_lat){
      found_lat = true;
      char* lat_num = (start_lat + 11);
      test_lat = trim_str(lat_num); 
    }
    char* end_long = strstr(mock_buffer, "}");
    if (start_long != NULL && end_long != NULL){
      char* long_num = (start_long + 13);
      char* test_long = trim_str(long_num);
      char *result = (char *) malloc(100);  // array to hold the result.
      strcpy(result,"GET /v1/forecast?latitude="); 
      strcat(result,test_lat);
      strcat(result,"&longitude=");
      strcat(result,test_long);
      strcat(result, "&current_weather=true HTTP/1.1");
      mock_response = result;
      return mock_response;
    }
  
  return default_location;
}

bool test_all_capstone_tests() {
   Serial.println("Calling capstone tests");
  
    // TRIM_STR TESTS
   // Testing that trim_str responds correctly to the possible latitude/longitude formats returned by the API,
   // which include positive, negative and with varying numbers of digits
   if (!test_trim("\"latitude\":-49.55,\"longitude\"","-49.55", 11)){ // negative latitude, 2 digits
    return false;
   }
   if (!test_trim("\"latitude\":49.55,\"longitude\"","49.55", 11)){ // positive latitude, 2 digits
    return false; 
   }
   if (!test_trim("\"latitude\":-9.55,\"longitude\"","-9.55", 11)){ // negative latitude, 1 digit
    return false;
   }
   if (!test_trim("\"latitude\":9.55,\"longitude\"","9.55", 11)){ // positive latitude, 1 digit
    return false;
   }
   if (!test_trim("\"latitude\":9.5555,\"longitude\"","9.5555", 11)){  // latitude w/extra decimals
    return false;
   }
   if(!test_trim( ",\"longitude\":9.12}\"", "9.12", 13)){ // positive longitude, one digit
    return false;
   }
   if(!test_trim( ",\"longitude\":19.12}\"", "19.12", 13)){ // positive longitude, two digits
    return false;
   }
    if(!test_trim( ",\"longitude\":19.1266}\"", "19.1266", 13)){ // positive longitude, extra decimals
    return false;
   }
      if(!test_trim( ",\"longitude\":-9.12}\"", "-9.12", 13)){ // negative longitude, one digit
    return false;
   }
   if(!test_trim( ",\"longitude\":-19.12}\"", "-19.12", 13)){ // negative longitude, two digits
    return false;
   }
    if(!test_trim( ",\"longitude\":-19.1266}\"", "-19.1266", 13)){ // negative longitude, extra decimals
    return false;
   }


    // READ_LOCATION_WEBPAGE TESTS
    // Tests to make sure that the read_location_webpage function integrates trim_str appropriately 
    // and responds correctly to a 404 response from the api (that no such name exists and defaults to
    // the default response

   // on well-formed input
   char* mock_buffer = "{\"latitude\":-49.55,\"longitude\":9.12}";
   char *expected_res = "GET /v1/forecast?latitude=-49.55&longitude=9.12&current_weather=true HTTP/1.1";
   if (!test_read_location(mock_buffer, expected_res)){
    return false;
   }
   // on does not exist input
   mock_buffer = "no such name has been registered";
   expected_res = "unavailable";
   if(!test_read_location(mock_buffer, expected_res)){
    return false;
   }
   Serial.println("All capstone tests pass!");
}
