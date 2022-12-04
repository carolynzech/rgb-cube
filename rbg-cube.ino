#include "cubeplex.h"

enum Weather {
  UNSUPPORTED,
  RAINY,
  SUNNY,
  THUNDERSTORM,
  CLOUDY
};

Weather weather_desc = UNSUPPORTED;

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

  lastISR = millis();
  Serial.println("Done initializing!");
}

/*
 * Determines if a speficic int token is found in a
 * list of int.
 *
 * token - the int that we are searching for
 * list - the array of ints that we are looking
 *        into to find the target
 *
 * returns an int: 1 if the int was found and 0 if it was not
 */
int is_in(int token, int *list[]) {
    int counter = 0;
    while (list[counter]) {
        if (list[counter] == token) return 1;
        counter++;
    }
    return 0;
}

/*
 * Updates the variable Weather input to the finite state machine.
 * Sets weather_desc based on the recieved weather_type from the API
 * call, which becomes the input to light_cube().
 * 
 * weather_type - the WMO Weather interpretation code recieved from the API
 * returns nothing. updates the global weather_desc.
 */
void update_fsm(int weather_type) {
  if is_in(weather_type, sun_list) {
    weather_desc = SUNNY;
  } else if is_in(weather_type, cloud_list) {
    weather_desc = CLOUDY;
  } else if is_in(weather_type, rain_list) {
    weather_desc = RAINY;
  } else if is_in(weather_type, tstorm_list) {
    weather_desc = THUNDERSTORM;
  } else {
    weather_desc = UNSUPPORTED;
  }
}

/*
 * Polls the weather API to update the global weather_desc according
 * to the short forecast recieved. This is expected to be called as
 * an interrupt. Prints to the Serial monitor on error.
 *
 * no inputs, returns nothing. updates the weather_desc global.
 */
void poll_data() {
  // call API
  int response = -1;
  while (response = read_webpage() < 0);

  update_fsm(response);
}

bool errorOccured = false;
int lastISR;

void TC1_Handler() {
  // Clear interrupt register flag
  // (use register TC3->COUNT16.register_name.reg)
  TC3->COUNT16.INTFLAG.bit.MC0 = 1;
  
  secs++; //Counter increases by 1 every second   

  errorOccured = true;
  lastISR = millis();

  if (secs >= 30) { // every 30 seconds
    poll_data();
    secs = 0;
  }
}

void WDT_Handler() {
  // Clear interrupt register flag
  WDT->INTFLAG.bit.EW = 1;
  
  // Warn user that a watchdog reset may happen
  Serial.println("Watchdog reset may happen!");
}

/*
 * Lights the RGB cube using the charlieCube library based on the current
 * weather reading.
 *
 * weather - the current weather
 * returns nothing; lights the cube
 */
void light_cube(Weather weather) {
  switch (weather) {
    case RAINY:
      // light blue
      break;
    case SUNNY: // clear skies
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

void print() {
  if (errorOccured) {
    int time = millis();
    Serial.print("ISR happened millis ago: ")
    Serial.println(time - lastISR);
    errorOccured = false;
  }
}

void loop() {
  light_cube(weather_desc);
  print();

  // pet watchdog
  WDT->CLEAR.reg = 0xa5;
}
