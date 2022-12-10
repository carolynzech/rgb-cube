// PORT A Group
int PIN2 = 10;
int PIN3 = 11;
int PIN6 = 20;
int PIN7 = 21;
int PIN8 = 16;
int PIN9 = 17;
int PIN10 = 19;
int PIN11 = 8;
int PIN12 = 9;
int PINA0 = 2;
int PINA3 = 4;

// PORT B Group
int PIN4 = 10;
int PIN5 = 11;
int PIN13 = 23;
int PINA1 = 2;
int PINA2 = 3;

enum Weather {
  UNSUPPORTED,
  RAINY,
  SUNNY,
  SNOWY,
  CLOUDY
};

enum Color {
  RED,
  GREEN,
  BLUE
};

volatile int poll_time;
volatile int prev_poll_time;
volatile int intcount = 6;

Weather weather_desc = UNSUPPORTED;

// translate weather codes to the supported weather patterns. terminate with -1
int sun_list[] = {0, 1, 2, -1};
int cloud_list[] = {3, 45, 48, -1};
int rain_list[] = {51, 53, 55, 56, 57, 61, 63, 65, 66, 67, 80, 81, 82, 95, 96, 99, -1};
int snow_list[] = {71, 73, 75, 77, 85, 86, -1};

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Initialized Serial!");
  
  setup_wifi();
  // Initialized Wifi!

  PORT->Group[PORTA].DIRSET.reg = (1 << PIN10); // wire 9
  PORT->Group[PORTB].DIRSET.reg = (1 << PIN4); // wire 3
  PORT->Group[PORTA].DIRSET.reg = (1 << PIN11); // wire 10
  PORT->Group[PORTB].DIRSET.reg = (1 << PIN5); // wire 4
  PORT->Group[PORTA].DIRSET.reg = (1 << PIN12); // wire 11
  PORT->Group[PORTB].DIRSET.reg = (1 << PIN13); // wire 12

  // wire 8, 7, 6, 5
  PORT->Group[PORTA].DIRSET.reg = (1 << PIN9);
  PORT->Group[PORTA].DIRSET.reg = (1 << PIN8);
  PORT->Group[PORTA].DIRSET.reg = (1 << PIN7);
  PORT->Group[PORTA].DIRSET.reg = (1 << PIN6);

  // wire 13, 14, 15, 16
  PORT->Group[PORTA].DIRSET.reg = (1 << PINA0);
  PORT->Group[PORTA].DIRSET.reg = (1 << PINA3);
  PORT->Group[PORTB].DIRSET.reg = (1 << PINA1);
  PORT->Group[PORTB].DIRSET.reg = (1 << PINA2);

  PORT->Group[PORTA].DIRSET.reg = (1 << PIN3); // wire 1
  PORT->Group[PORTA].DIRSET.reg = (1 << PIN2); // wire 2
  // Configure and enable GCLK4 for TC:
  GCLK->GENDIV.reg = GCLK_GENDIV_DIV(0) | GCLK_GENDIV_ID(4); // do not divide gclk 4
  while(GCLK->STATUS.bit.SYNCBUSY);
  // use GCLK->GENCTRL.reg and GCLK->CLKCTRL.reg
  GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(0x04) | GCLK_GENCTRL_IDC | GCLK_GENCTRL_SRC(0x06) | GCLK_GENCTRL_GENEN;
  while(GCLK->STATUS.bit.SYNCBUSY);
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(0x4) | GCLK_CLKCTRL_ID(0x1b) | GCLK_CLKCTRL_CLKEN;

  // Set up NVIC:
  NVIC_SetPriority(TC3_IRQn, 0);
  NVIC_EnableIRQ(TC3_IRQn);

  Serial.println("Initialized Timer!");

  // Clear and enable WDT
  // Early-warning interrupt
  NVIC_DisableIRQ(WDT_IRQn);
  NVIC_ClearPendingIRQ(WDT_IRQn);
  NVIC_SetPriority(WDT_IRQn, 0); //top priority
  NVIC_EnableIRQ(WDT_IRQn);

  // Configure and enable WDT GCLK:
  GCLK->GENDIV.reg = GCLK_GENDIV_DIV(4) | GCLK_GENDIV_ID(5);
  while (GCLK->STATUS.bit.SYNCBUSY);
  GCLK->GENCTRL.reg = GCLK_GENCTRL_DIVSEL | GCLK_GENCTRL_ID(0x5) | GCLK_GENCTRL_SRC(0x03) | GCLK_GENCTRL_GENEN;
  while (GCLK->STATUS.bit.SYNCBUSY);
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(0x5) | GCLK_CLKCTRL_ID(0x03);

  // Configure and enable WDT:
  WDT->CONFIG.reg = 0xB;

  // Enable early warning interrupts on WDT:
  WDT->EWCTRL.reg = 0x8;
  WDT->CTRL.reg = WDT_CTRL_ENABLE;
  while (WDT->STATUS.bit.SYNCBUSY);
  WDT->INTENSET.bit.EW = 1;
  Serial.println("Initialized Watchdog Timer!");

  
  poll_time = millis();
  prev_poll_time = millis();

  // Enable the timer/counter
  // Turn off interrupts to TC3 on MC0 when configuring
  TC3->COUNT16.INTENCLR.bit.MC0 = 1;
  while(TC3->COUNT16.STATUS.bit.SYNCBUSY);

  // configure TC
  TC3->COUNT16.CTRLA.bit.MODE = 0x0;
  TC3->COUNT16.CTRLA.bit.PRESCALER = 0x1f;
  TC3->COUNT16.CTRLA.bit.PRESCSYNC = 0x1;
  TC3->COUNT16.CTRLA.bit.WAVEGEN = 0x1;
  TC3->COUNT16.CTRLA.bit.ENABLE = 1;
  TC3->COUNT16.CC[0].reg = 6000000;
  
  // Turn interrupts to TC3 on MC0 back on when done configuring
  TC3->COUNT16.INTENSET.bit.MC0 = 1;
  while(TC3->COUNT16.STATUS.bit.SYNCBUSY);
  
  Serial.println("Done initializing!");
}

/*
 * Determines if a speficic int token is found in a
 * list of int. Assumes the list is terminated with -1.
 *
 * token - the int that we are searching for
 * list - the array of ints that we are looking
 *        into to find the target
 *
 * returns an int: 1 if the int was found and 0 if it was not
 */
int is_in(int token, int list[]) {
    int counter = 0;
    while (list[counter] != -1) {
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
  // Serial.println("Weather type is: ");
  // Serial.println(weather_type);
  if (is_in(weather_type, sun_list)) {
    weather_desc = SUNNY;
  } else if (is_in(weather_type, cloud_list)) {
    weather_desc = CLOUDY;
  } else if (is_in(weather_type, rain_list)) {
    weather_desc = RAINY;
  } else if (is_in(weather_type, snow_list)) {
    weather_desc = SNOWY;
  } else {
    weather_desc = UNSUPPORTED;
  }
  // Serial.println("Weather desc is: ");
  // Serial.println(weather_desc);
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
  int response = read_webpage();
  int try_read = millis();
  if (response == -1) {
    Serial.println("Failed to read weather... trying again.");
  } else {
    poll_time = millis();
    Serial.print("Data polled! Last poll was ");
    Serial.print((poll_time - prev_poll_time) / 100);
    Serial.println(" seconds ago.");
    update_fsm(response);
    prev_poll_time = poll_time;
    intcount = 0; // reset counter since we successfully polled
  }
}

void TC3_Handler() {
  // Clear interrupt register flag
  // (use register TC3->COUNT16.register_name.reg)
  TC3->COUNT16.INTFLAG.bit.MC0 = 1;

  // print the current weathter status
  // Serial.print("Current weather: ");
  // Serial.println(weather_desc);
  
  intcount++; // counter increases every approx. 4.6 seconds
  if (intcount >= 5) { // after approx 23 seconds since last successful poll
      poll_data();
  }
}

void WDT_Handler() {
  // Clear interrupt register flag
  WDT->INTFLAG.reg = 1;
  
  // Warn user that a watchdog reset may happen
  Serial.println("Watchdog reset may happen!");
}
int pattern_counter = 0;
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
      all_layers_solid(BLUE, 200);
      break;
    case SUNNY: // clear skies
      all_layers_solid(GREEN, 1);
      break;
    case SNOWY:
      make_rainbow(1);
      break;
    case CLOUDY:
      cloudy_pwm();
      break;
    default:
      turn_lights_off();
  }
}

int i=0;

void loop() {
  // pet watchdog
  WDT->CLEAR.reg = 0xa5;

  light_cube(weather_desc);
  check_connection();
  
}
