#include <assert.h>

void test_all_tests() {
  assert(test_is_in());
  assert(test_update_fsm());
  assert(test_TC3_Handler());
  assert(test_get_weather_from_time());
  assert(test_light_cube());

  Serial.println("All tests passed!");
}

bool test_is_in() {
  int full_list[] = {1, 2, 3, -1};
  int empty_list[] = {-1};
  int single_list[] = {10, -1};
  int repeat_list[] = {1, 1, 2, 2, 3, 3, -1, -1};
  assert(is_in(2, full_list));
  assert(is_in(1, full_list));
  assert(is_in(3, full_list));
  assert(!is_in(15, full_list));
  assert(!is_in(15, empty_list));
  assert(is_in(1, repeat_list));
  assert(is_in(3, repeat_list));
  assert(!is_in(4, repeat_list));
  assert(is_in(10, single_list));
  assert(!is_in(-10, single_list));

  Serial.println("Passed test_is_in!");
  return true;
}

bool test_update_fsm() {
  // starting in unsupported
  weather_desc = UNSUPPORTED; 
  update_fsm(-2);
  assert(weather_desc == UNSUPPORTED);

  weather_desc = UNSUPPORTED; 
  update_fsm(rain_list[0]);
  assert(weather_desc == RAINY);

  weather_desc = UNSUPPORTED; 
  update_fsm(sun_list[0]);
  assert(weather_desc == CLEAR);

  weather_desc = UNSUPPORTED; 
  update_fsm(snow_list[0]);
  assert(weather_desc == SNOWY);

  weather_desc = UNSUPPORTED; 
  update_fsm(cloud_list[0]);
  assert(weather_desc == CLOUDY);

  // starting in rainy
  weather_desc = RAINY; 
  update_fsm(-2);
  assert(weather_desc == UNSUPPORTED);

  weather_desc = RAINY; 
  update_fsm(rain_list[0]);
  assert(weather_desc == RAINY);

  weather_desc = RAINY; 
  update_fsm(sun_list[0]);
  assert(weather_desc == CLEAR);

  weather_desc = RAINY; 
  update_fsm(snow_list[0]);
  assert(weather_desc == SNOWY);

  weather_desc = RAINY; 
  update_fsm(cloud_list[0]);
  assert(weather_desc == CLOUDY);
  
  // starting in CLEAR
  weather_desc = CLEAR; 
  update_fsm(-2);
  assert(weather_desc == UNSUPPORTED);

  weather_desc = CLEAR; 
  update_fsm(rain_list[0]);
  assert(weather_desc == RAINY);

  weather_desc = CLEAR; 
  update_fsm(sun_list[0]);
  assert(weather_desc == CLEAR);

  weather_desc = CLEAR; 
  update_fsm(snow_list[0]);
  assert(weather_desc == SNOWY);

  weather_desc = CLEAR; 
  update_fsm(cloud_list[0]);
  assert(weather_desc == CLOUDY);

  // starting in snowy
  weather_desc = SNOWY; 
  update_fsm(-2);
  assert(weather_desc == UNSUPPORTED);

  weather_desc = SNOWY; 
  update_fsm(rain_list[0]);
  assert(weather_desc == RAINY);

  weather_desc = SNOWY; 
  update_fsm(sun_list[0]);
  assert(weather_desc == CLEAR);

  weather_desc = SNOWY; 
  update_fsm(snow_list[0]);
  assert(weather_desc == SNOWY);

  weather_desc = SNOWY; 
  update_fsm(cloud_list[0]);
  assert(weather_desc == CLOUDY);

  // starting in cloudy
  weather_desc = CLOUDY; 
  update_fsm(cloud_list[0]);
  assert(weather_desc == CLOUDY);

  weather_desc = CLOUDY; 
  update_fsm(rain_list[0]);
  assert(weather_desc == RAINY);

  weather_desc = CLOUDY; 
  update_fsm(sun_list[0]);
  assert(weather_desc == CLEAR);

  weather_desc = CLOUDY; 
  update_fsm(snow_list[0]);
  assert(weather_desc == SNOWY);

  weather_desc = CLOUDY; 
  update_fsm(-5);
  assert(weather_desc == UNSUPPORTED);
  
  Serial.println("Passed test_update_fsm!");
  return true;
}

bool test_TC3_Handler() {
  // Initialize and start timer
  GCLK->GENDIV.reg = GCLK_GENDIV_DIV(0) | GCLK_GENDIV_ID(4); // do not divide gclk 4
  while(GCLK->STATUS.bit.SYNCBUSY);
  // use GCLK->GENCTRL.reg and GCLK->CLKCTRL.reg
  GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(0x04) | GCLK_GENCTRL_IDC | GCLK_GENCTRL_SRC(0x06) | GCLK_GENCTRL_GENEN;
  while(GCLK->STATUS.bit.SYNCBUSY);
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(0x4) | GCLK_CLKCTRL_ID(0x1b) | GCLK_CLKCTRL_CLKEN;
  // Set up NVIC:
  NVIC_SetPriority(TC3_IRQn, 0);
  NVIC_EnableIRQ(TC3_IRQn);
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
  // done setup

  // start millis counter
  intcount = 0;
  int start_mils = millis();
  
  // wait until intcount == 5
  while (intcount < 5);

  // check that millis is > 20 seconds
  int curr_mils = millis();
  assert(curr_mils - start_mils > 20000);
  
  // check that global has been set by poll_data mock
  assert(mock_poll_success);
 
  // Disable TC
  TC3->COUNT16.INTENCLR.bit.MC0 = 1;
  TC3->COUNT16.CTRLA.bit.ENABLE = 0;
  while(TC3->COUNT16.STATUS.bit.SYNCBUSY);

  Serial.println("Passed test_TC3_Handler!");
  return true;
}

bool test_get_weather_from_time() {
  // single digit code
  char *overcast = ".0,\"weathercode\":3,\"time\":";
  char *time_ptr = strstr(overcast, "time\":");
  assert(get_weather_from_time(time_ptr) == 3);

  char *mainly_clear = ":1,\"time\":";
  time_ptr = strstr(mainly_clear, "time\":");
  assert(get_weather_from_time(time_ptr) == 1);

  // code is 0
  char *clear_sky = "code\":0,\"time\":";
  time_ptr = strstr(clear_sky, "time\":");
  assert(get_weather_from_time(time_ptr) == 0);
  
  // double digit code
  char *fog = "45,\"time\":";
  time_ptr = strstr(fog, "time\":");
  assert(get_weather_from_time(time_ptr) == 45);

  char *snow_grains = "\"weathercode\":77,\"time\":";
  time_ptr = strstr(snow_grains, "time\":");
  assert(get_weather_from_time(time_ptr) == 77);

  // unsupported weathe found
  char *unsupported = "\"weathercode\":58,\"time\":";
  time_ptr = strstr(unsupported, "time\":");
  assert(get_weather_from_time(time_ptr) == 58);

  // not a number
  char *nan = "\"weathercode\":,\"time\":";
  time_ptr = strstr(nan, "time\":");
  assert(get_weather_from_time(time_ptr) == -1);

  // time was not found in response
  assert(get_weather_from_time(NULL) == -1);

  Serial.println("Passed test_get_weather_from_time!");
  return true;
}


bool test_light_cube() {
  // Since we cannot do automating testing GPIO output, we are instead
  // testing that the correct helper fucnctions are called with the correct inputs

  // rainy case
  light_cube(RAINY);
  assert(called_top_layer_solid);
  assert(top_layer_color == BLUE);
  assert(top_layer_delay == 200);
  assert(called_third_layer_solid);
  assert(third_layer_color == BLUE);
  assert(third_layer_delay == 200);
  assert(called_second_layer_solid);
  assert(second_layer_color == BLUE);
  assert(second_layer_delay == 200);
  assert(called_bottom_layer_solid);
  assert(bottom_layer_color == BLUE);
  assert(bottom_layer_delay == 200);
  // reset globals
  called_top_layer_solid = false;
  top_layer_delay = NULL;
  called_third_layer_solid = false;
  third_layer_delay = NULL;
  called_second_layer_solid = false;
  second_layer_delay = NULL;
  called_bottom_layer_solid = false;
  bottom_layer_delay = NULL;

  // clear sikes case
  light_cube(CLEAR);
  assert(called_top_layer_solid);
  assert(top_layer_color == GREEN);
  assert(top_layer_delay == 1);
  assert(called_third_layer_solid);
  assert(third_layer_color == GREEN);
  assert(third_layer_delay == 1);
  assert(called_second_layer_solid);
  assert(second_layer_color == GREEN);
  assert(second_layer_delay == 1);
  assert(called_bottom_layer_solid);
  assert(bottom_layer_color == GREEN);
  assert(bottom_layer_delay == 1);
  // reset globals
  called_top_layer_solid = false;
  top_layer_delay = NULL;
  called_third_layer_solid = false;
  third_layer_delay = NULL;
  called_second_layer_solid = false;
  second_layer_delay = NULL;
  called_bottom_layer_solid = false;
  bottom_layer_delay = NULL;

  // snowy case
  light_cube(SNOWY);
  assert(cube_made_white);
  assert(cube_made_white_delay == 1);
  //r reset globals
  cube_made_white = false;
  cube_made_white_delay = NULL;

  // cloudy case
  light_cube(CLOUDY);
  assert(cube_pwm_sent);
  cube_pwm_sent = false;

  // other cases (unsupported)
  light_cube(UNSUPPORTED);
  assert(cube_turned_off);
  cube_turned_off = false;

  Serial.println("Passed test_light_cube!");
  return true;
}
