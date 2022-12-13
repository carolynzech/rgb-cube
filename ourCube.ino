#include "cubeplex.h"
#include "rgb.h"

int color = red;
Weather weather_desc = UNSUPPORTED;
jsmn_parser parser;

int[] sun_list = [0, 1, 2];
int[] cloud_list = [3, 45, 48];
int[] rain_list = [51, 53, 55, 56, 57, 61, 63, 65, 66, 67, 80, 81, 82];
int[] tstorm_list = [95, 96, 99];

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Initialized Serial!");


  initCube();
  Serial.println("Initialized Cube!");


  jsmn_init(&parser);
  Serial.println("Initialized Parser!");


  // Configure and enable GCLK4 for TC:
  GCLK->GENDIV.reg = GCLK_GENDIV_DIV(0) | GCLK_GENDIV_ID(4); // do not divide gclk 4
  while(GCLK->STATUS.bit.SYNCBUSY);
  // use GCLK->GENCTRL.reg and GCLK->CLKCTRL.reg
  GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(0x04) | GCLK_GENCTRL_IDC | GCLK_GENCTRL_SRC(0x06) | GCLK_GENCTRL_GENEN;
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(0x4) | GCLK_CLKCTRL_ID(0x1b) | GCLK_CLKCTRL_CLKEN;
  while(GCLK->STATUS.bit.SYNCBUSY);

  // Check if APB is enabled:
  Serial.println(PM->APBCMASK.reg & PM_APBCMASK_TC3, BIN);
  // use PM->APBX.reg (change X to the correct letter and mask the relevant bit)

  // Disable TC (for now)
  // use TC3->COUNT16.CTRLA.reg and TC3->COUNT16.INTENCLR.reg
  TC3->COUNT16.INTENCLR.bit.MC0 = 1;
  TC3->COUNT16.CTRLA.bit.ENABLE = 0;
  while(TC3->COUNT16.STATUS.bit.SYNCBUSY);

  // Set up NVIC:
  NVIC_SetPriority(TC3_IRQn, 0);
  NVIC_EnableIRQ(TC3_IRQn);

  Serial.println("Initialized Timer!");


  // Clear and enable WDT
  NVIC_DisableIRQ(WDT_IRQn);
  NVIC_ClearPendingIRQ(WDT_IRQn);
  NVIC_SetPriority(WDT_IRQn, 0);
  NVIC_EnableIRQ(WDT_IRQn);

  // Configure and enable WDT GCLK:
  GCLK->GENDIV.reg = GCLK_GENDIV_DIV(4) | GCLK_GENDIV_ID(5);
  while (GCLK->STATUS.bit.SYNCBUSY);
  GCLK->GENCTRL.reg = GCLK_GENCTRL_DIVSEL | GCLK_GENCTRL_ID(0x5) | GCLK_GENCTRL_SRC(0x03) | GCLK_GENCTRL_GENEN;
  while (GCLK->STATUS.bit.SYNCBUSY);
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(0x5) | GCLK_CLKCTRL_ID(0x03) | GCLK_CLKCTRL_CLKEN;

  // Configure and enable WDT:
  WDT->CONFIG.reg = 0x9;
  WDT->EWCTRL.reg = 0x8;
  WDT->CTRL.reg = WDT_CTRL_ENABLE;
  while (WDT->STATUS.bit.SYNCBUSY);

  // Enable early warning interrupts on WDT:
  WDT->INTENSET.reg = WDT_INTENSET_EW;

  // reference WDT registers with WDT->register_name.reg
  Serial.println("Initialized Watchdog!");

  Serial.println("Done initializing!");
}

/*
 * Parses the API response in order to get the appropriate weather_desc from the
 * short forecast. Returns the success of the parse.
 *
 * json - the JSON response from the API
 * returns 0 on success, -1 on error. sets the global weather_desc
 */
int parseWeather(const char *json) {
  jsmntok_t tokens[256]; //TODO how many tokens are needed
  int jsonLen = jsmn_parse(&parser, json, strlen(json), tokens, 256);
  if (jsonLen == JSMN_ERROR_NOMEM) {
    // TODO: remove when we know the size
    Serial.println("we need more size");
    return -1;
  }

  // Error check the response from the parser
  if (jsonLen < 0) {
    Serial.println("Error: JSON was corrupted or ended before it was expected");
    return -1;
  }

  // The top-level element must be an object
  if (jsonLen < 1 || t[0].type != JSMN_OBJECT) {
    Serial.println("Error: expected JSON to have object at top-level");
    return -1;
  }

  // Loop over all keys of the root object
  for (int i = 1; i < jsonLen; i++) {
    if (jsoneq(json, &tokens[i], "user") == 0) { //TODO: know the structure of the data before parsing
      /* We may use strndup() to fetch string value */
      printf("- User: %.*s\n", t[i + 1].end - t[i + 1].start, json + t[i + 1].start);
      i++;
    }
  }
}

/*
 * Polls the weather API to update the global weather_desc according
 * to the short forecast recieved. This is expected to be called as
 * an interrupt. Prints to the Serial monitor on error.
 *
 * no inputs, returns nothing. updates the weather_desc global.
 */
void pollData() {
  // call API
  // get JSON as string
  char *response;

  // parse to JSON object
  if (parseWeather(response) < 0) {
    Serial.println("Error: could not parse forecast from JSON recieved:");
    Serial.println(response);
  }
}

/*
 * Lights the RGB cube using the charlieCube library based on the current
 * weather reading.
 *
 * weather - the current weather
 * returns nothing; lights the cube
 */
void lightCube(Weather weather) {
  switch (weather) {
    case RAINY:
      // light blue
      break;
    case SUNNY:
      // light yellow
      break;
    case THUNDERSTORM:
      // light thunderstorm
      break;
    case CLOUDY:
      // light cloudy
      break;
    default:
      // light off
      break;
  }
}

void loop() {
  lightCube(weather_desc);
  // pet watchdog
}
