void turn_lights_off() {
  PORT->Group[PORTA].OUTCLR.reg = (1 << PIN10); // wire 9
  PORT->Group[PORTB].OUTCLR.reg = (1 << PIN4); // wire 3
  PORT->Group[PORTA].OUTCLR.reg = (1 << PIN11); // wire 10
  PORT->Group[PORTB].OUTCLR.reg = (1 << PIN5); // wire 4
  PORT->Group[PORTA].OUTCLR.reg = (1 << PIN12); // wire 11
  PORT->Group[PORTB].OUTCLR.reg = (1 << PIN13); // wire 12

  // wire 8, 7, 6, 5
  PORT->Group[PORTA].OUTCLR.reg = (1 << PIN9);
  PORT->Group[PORTA].OUTCLR.reg = (1 << PIN8);
  PORT->Group[PORTA].OUTCLR.reg = (1 << PIN7);
  PORT->Group[PORTA].OUTCLR.reg = (1 << PIN6);

  // wire 13, 14, 15, 16
  PORT->Group[PORTA].OUTCLR.reg = (1 << PINA0);
  PORT->Group[PORTA].OUTCLR.reg = (1 << PINA3);
  PORT->Group[PORTB].OUTCLR.reg = (1 << PINA1);
  PORT->Group[PORTB].OUTCLR.reg = (1 << PINA2);

  PORT->Group[PORTA].OUTCLR.reg = (1 << PIN3); // wire 1
  PORT->Group[PORTA].OUTCLR.reg = (1 << PIN2); // wire 2
}

void bottom_layer_solid(Color color, int delay_time) {
  // set ground legs
  PORT->Group[PORTA].DIRSET.reg = (1 << PIN2);
  PORT->Group[PORTA].DIRSET.reg = (1 << PIN3);
  PORT->Group[PORTB].DIRSET.reg = (1 << PIN4);
  PORT->Group[PORTB].DIRSET.reg = (1 << PIN5);

  PORT->Group[PORTA].OUTCLR.reg = (1 << PIN2);
  PORT->Group[PORTA].OUTCLR.reg = (1 << PIN3);
  PORT->Group[PORTB].OUTCLR.reg = (1 << PIN4);
  PORT->Group[PORTB].OUTCLR.reg = (1 << PIN5);
  
  switch(color) {
    
    case(BLUE):
      // charlieplexing: set pins uninvolved with bottom layer being red to INPUT
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN6);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN7); 
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN8); 
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN9);  
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN10);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN11);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN12);
      PORT->Group[PORTB].DIRCLR.reg = (1 << PIN13); 

      // drive red pins high
      PORT->Group[PORTA].DIRSET.reg = (1 << PINA0);
      PORT->Group[PORTB].DIRSET.reg = (1 << PINA1);
      PORT->Group[PORTB].DIRSET.reg = (1 << PINA2);
      PORT->Group[PORTA].DIRSET.reg = (1 << PINA3);

      PORT->Group[PORTA].OUTSET.reg = (1 << PINA0);
      PORT->Group[PORTB].OUTSET.reg = (1 << PINA1);
      PORT->Group[PORTB].OUTSET.reg = (1 << PINA2);
      PORT->Group[PORTA].OUTSET.reg = (1 << PINA3);
      break;
    
    case(GREEN):
      // charlieplexing: set pins uninvolved with bottom layer being green to INPUT
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN6);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN7); 
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN8); 
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN9);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PINA0);
      PORT->Group[PORTB].DIRCLR.reg = (1 << PINA1);
      PORT->Group[PORTB].DIRCLR.reg = (1 << PINA2);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PINA3);
      
      // drive green pins high
      PORT->Group[PORTA].DIRSET.reg = (1 << PIN10);
      PORT->Group[PORTA].DIRSET.reg = (1 << PIN11);
      PORT->Group[PORTA].DIRSET.reg = (1 << PIN12);
      PORT->Group[PORTB].DIRSET.reg = (1 << PIN13);

      PORT->Group[PORTA].OUTSET.reg = (1 << PIN10);
      PORT->Group[PORTA].OUTSET.reg = (1 << PIN11);
      PORT->Group[PORTA].OUTSET.reg = (1 << PIN12);
      PORT->Group[PORTB].OUTSET.reg = (1 << PIN13);
      break;

      case(RED):
      // charlieplexing: set pins uninvolved with bottom layer being blue to INPUT
      PORT->Group[PORTA].DIRCLR.reg = (1 << PINA0);
      PORT->Group[PORTB].DIRCLR.reg = (1 << PINA1);
      PORT->Group[PORTB].DIRCLR.reg = (1 << PINA2);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PINA3);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN10);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN11);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN12);
      PORT->Group[PORTB].DIRCLR.reg = (1 << PIN13); 

      // drive blue pins high
      PORT->Group[PORTA].DIRSET.reg = (1 << PIN6);
      PORT->Group[PORTA].DIRSET.reg = (1 << PIN7); 
      PORT->Group[PORTA].DIRSET.reg = (1 << PIN8); 
      PORT->Group[PORTA].DIRSET.reg = (1 << PIN9);

      PORT->Group[PORTA].OUTSET.reg = (1 << PIN6);
      PORT->Group[PORTA].OUTSET.reg = (1 << PIN7); 
      PORT->Group[PORTA].OUTSET.reg = (1 << PIN8); 
      PORT->Group[PORTA].OUTSET.reg = (1 << PIN9);
      break;
    default:
      break;
  }
  delay(delay_time);
}

void second_layer_solid(Color color, int delay_time) {
  // set ground legs
  PORT->Group[PORTA].DIRSET.reg = (1 << PINA0);
  PORT->Group[PORTB].DIRSET.reg = (1 << PINA1);
  PORT->Group[PORTB].DIRSET.reg = (1 << PINA2);
  PORT->Group[PORTA].DIRSET.reg = (1 << PINA3);  

  PORT->Group[PORTA].OUTCLR.reg = (1 << PINA0);
  PORT->Group[PORTB].OUTCLR.reg = (1 << PINA1);
  PORT->Group[PORTB].OUTCLR.reg = (1 << PINA2);
  PORT->Group[PORTA].OUTCLR.reg = (1 << PINA3);  
  
  switch (color){
    case (RED):
    // charlieplex uninvolved
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN10);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN11);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN12);
      PORT->Group[PORTB].DIRCLR.reg = (1 << PIN13); 
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN6);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN7); 
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN8); 
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN9);

    // drive red legs
    PORT->Group[PORTA].DIRSET.reg = (1 << PIN2);
    PORT->Group[PORTA].DIRSET.reg = (1 << PIN3);
    PORT->Group[PORTB].DIRSET.reg = (1 << PIN4);
    PORT->Group[PORTB].DIRSET.reg = (1 << PIN5);
    
    PORT->Group[PORTA].OUTSET.reg = (1 << PIN2);
    PORT->Group[PORTA].OUTSET.reg = (1 << PIN3);
    PORT->Group[PORTB].OUTSET.reg = (1 << PIN4);
    PORT->Group[PORTB].OUTSET.reg = (1 << PIN5);
    break;
    case (GREEN):
    // charlieplex uninvolved 
    PORT->Group[PORTA].DIRCLR.reg = (1 << PIN10);
    PORT->Group[PORTA].DIRCLR.reg = (1 << PIN11);
    PORT->Group[PORTA].DIRCLR.reg = (1 << PIN12);
    PORT->Group[PORTB].DIRCLR.reg = (1 << PIN13); 
    PORT->Group[PORTA].DIRCLR.reg = (1 << PIN2);
    PORT->Group[PORTA].DIRCLR.reg = (1 << PIN3);
    PORT->Group[PORTB].DIRCLR.reg = (1 << PIN4);
    PORT->Group[PORTB].DIRCLR.reg = (1 << PIN5);


    //drive green legs
      PORT->Group[PORTA].DIRSET.reg = (1 << PIN6);
      PORT->Group[PORTA].DIRSET.reg = (1 << PIN7); 
      PORT->Group[PORTA].DIRSET.reg = (1 << PIN8); 
      PORT->Group[PORTA].DIRSET.reg = (1 << PIN9);

      PORT->Group[PORTA].OUTSET.reg = (1 << PIN6);
      PORT->Group[PORTA].OUTSET.reg = (1 << PIN7); 
      PORT->Group[PORTA].OUTSET.reg = (1 << PIN8); 
      PORT->Group[PORTA].OUTSET.reg = (1 << PIN9);
      break;
    case (BLUE): 
      // charlieplex uninvolved
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN2);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN3);
      PORT->Group[PORTB].DIRCLR.reg = (1 << PIN4);
      PORT->Group[PORTB].DIRCLR.reg = (1 << PIN5);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN6);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN7); 
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN8); 
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN9);

      //drive blue legs
      PORT->Group[PORTA].DIRSET.reg = (1 << PIN10);
      PORT->Group[PORTA].DIRSET.reg = (1 << PIN11);
      PORT->Group[PORTA].DIRSET.reg = (1 << PIN12);
      PORT->Group[PORTB].DIRSET.reg = (1 << PIN13); 

      PORT->Group[PORTA].OUTSET.reg = (1 << PIN10);
      PORT->Group[PORTA].OUTSET.reg = (1 << PIN11);
      PORT->Group[PORTA].OUTSET.reg = (1 << PIN12);
      PORT->Group[PORTB].OUTSET.reg = (1 << PIN13); 
      break;
    default:
      break;
  }
  delay(delay_time);
}

void third_layer_solid(Color color, int delay_time) {
  // set ground legs

      PORT->Group[PORTA].DIRSET.reg = (1 << PIN10);
      PORT->Group[PORTA].DIRSET.reg = (1 << PIN11);
      PORT->Group[PORTA].DIRSET.reg = (1 << PIN12);
      PORT->Group[PORTB].DIRSET.reg = (1 << PIN13);

      PORT->Group[PORTA].OUTCLR.reg = (1 << PIN10);
      PORT->Group[PORTA].OUTCLR.reg = (1 << PIN11);
      PORT->Group[PORTA].OUTCLR.reg = (1 << PIN12);
      PORT->Group[PORTB].OUTCLR.reg = (1 << PIN13);

  switch(color) {
    case(RED):
    // charlieplex uninvolved
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN2);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN3);
      PORT->Group[PORTB].DIRCLR.reg = (1 << PIN4);
      PORT->Group[PORTB].DIRCLR.reg = (1 << PIN5);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN6);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN7); 
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN8); 
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN9);  

    // drive red legs
      PORT->Group[PORTA].DIRSET.reg = (1 << PINA0);
      PORT->Group[PORTB].DIRSET.reg = (1 << PINA1);
      PORT->Group[PORTB].DIRSET.reg = (1 << PINA2);
      PORT->Group[PORTA].DIRSET.reg = (1 << PINA3);

      PORT->Group[PORTA].OUTSET.reg = (1 << PINA0);
      PORT->Group[PORTB].OUTSET.reg = (1 << PINA1);
      PORT->Group[PORTB].OUTSET.reg = (1 << PINA2);
      PORT->Group[PORTA].OUTSET.reg = (1 << PINA3);
      break;

    case(GREEN):
    // charlieplex uninvolved
    PORT->Group[PORTA].DIRCLR.reg = (1 << PIN6);
    PORT->Group[PORTA].DIRCLR.reg = (1 << PIN7); 
    PORT->Group[PORTA].DIRCLR.reg = (1 << PIN8); 
    PORT->Group[PORTA].DIRCLR.reg = (1 << PIN9);
    PORT->Group[PORTA].DIRCLR.reg = (1 << PINA0);
    PORT->Group[PORTB].DIRCLR.reg = (1 << PINA1);
    PORT->Group[PORTB].DIRCLR.reg = (1 << PINA2);
    PORT->Group[PORTA].DIRCLR.reg = (1 << PINA3);  

    //drive green legs 
    PORT->Group[PORTA].DIRSET.reg = (1 << PIN2);
    PORT->Group[PORTA].DIRSET.reg = (1 << PIN3);
    PORT->Group[PORTB].DIRSET.reg = (1 << PIN4);
    PORT->Group[PORTB].DIRSET.reg = (1 << PIN5);


    PORT->Group[PORTA].OUTSET.reg = (1 << PIN2);
    PORT->Group[PORTA].OUTSET.reg = (1 << PIN3);
    PORT->Group[PORTB].OUTSET.reg = (1 << PIN4);
    PORT->Group[PORTB].OUTSET.reg = (1 << PIN5);
    break;
    case(BLUE):
    // charlieplex uninvolved
    PORT->Group[PORTA].DIRCLR.reg = (1 << PINA0);
    PORT->Group[PORTB].DIRCLR.reg = (1 << PINA1);
    PORT->Group[PORTB].DIRCLR.reg = (1 << PINA2);
    PORT->Group[PORTA].DIRCLR.reg = (1 << PINA3);
    PORT->Group[PORTA].DIRCLR.reg = (1 << PIN2);
    PORT->Group[PORTA].DIRCLR.reg = (1 << PIN3);
    PORT->Group[PORTB].DIRCLR.reg = (1 << PIN4);
    PORT->Group[PORTB].DIRCLR.reg = (1 << PIN5);

    //drive blue legs
    PORT->Group[PORTA].DIRSET.reg = (1 << PIN6);
    PORT->Group[PORTA].DIRSET.reg = (1 << PIN7); 
    PORT->Group[PORTA].DIRSET.reg = (1 << PIN8); 
    PORT->Group[PORTA].DIRSET.reg = (1 << PIN9);

    PORT->Group[PORTA].OUTSET.reg = (1 << PIN6);
    PORT->Group[PORTA].OUTSET.reg = (1 << PIN7); 
    PORT->Group[PORTA].OUTSET.reg = (1 << PIN8); 
    PORT->Group[PORTA].OUTSET.reg = (1 << PIN9);
    break;
    
    default:
      break;
  }
  delay(delay_time);
}

void top_layer_solid(Color color, int delay_time) {
    // set ground legs

    PORT->Group[PORTA].DIRSET.reg = (1 << PIN6);
    PORT->Group[PORTA].DIRSET.reg = (1 << PIN7); 
    PORT->Group[PORTA].DIRSET.reg = (1 << PIN8); 
    PORT->Group[PORTA].DIRSET.reg = (1 << PIN9);  
    PORT->Group[PORTA].OUTCLR.reg = (1 << PIN6);
    PORT->Group[PORTA].OUTCLR.reg = (1 << PIN7); 
    PORT->Group[PORTA].OUTCLR.reg = (1 << PIN8); 
    PORT->Group[PORTA].OUTCLR.reg = (1 << PIN9);  
  
  switch(color) {
    
    case(RED):
      // charlieplexing: set pins uninvolved with bottom layer being red to INPUT
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN2);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN3);
      PORT->Group[PORTB].DIRCLR.reg = (1 << PIN4);
      PORT->Group[PORTB].DIRCLR.reg = (1 << PIN5);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PINA0);
      PORT->Group[PORTB].DIRCLR.reg = (1 << PINA1);
      PORT->Group[PORTB].DIRCLR.reg = (1 << PINA2);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PINA3);
      
      // drive red pins high

      PORT->Group[PORTA].DIRSET.reg = (1 << PIN10);
      PORT->Group[PORTA].DIRSET.reg = (1 << PIN11);
      PORT->Group[PORTA].DIRSET.reg = (1 << PIN12);
      PORT->Group[PORTB].DIRSET.reg = (1 << PIN13); 
      PORT->Group[PORTA].OUTSET.reg = (1 << PIN10);
      PORT->Group[PORTA].OUTSET.reg = (1 << PIN11);
      PORT->Group[PORTA].OUTSET.reg = (1 << PIN12);
      PORT->Group[PORTB].OUTSET.reg = (1 << PIN13); 
      
      break;
    
    case(GREEN):
      // charlieplexing: set pins uninvolved with bottom layer being green to INPUT
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN2);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN3);
      PORT->Group[PORTB].DIRCLR.reg = (1 << PIN4);
      PORT->Group[PORTB].DIRCLR.reg = (1 << PIN5);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN10);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN11);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN12);
      PORT->Group[PORTB].DIRCLR.reg = (1 << PIN13);
      
      // drive green pins high
      PORT->Group[PORTA].DIRSET.reg = (1 << PINA0);
      PORT->Group[PORTB].DIRSET.reg = (1 << PINA1);
      PORT->Group[PORTB].DIRSET.reg = (1 << PINA2);
      PORT->Group[PORTA].DIRSET.reg = (1 << PINA3);
      PORT->Group[PORTA].OUTSET.reg = (1 << PINA0);
      PORT->Group[PORTB].OUTSET.reg = (1 << PINA1);
      PORT->Group[PORTB].OUTSET.reg = (1 << PINA2);
      PORT->Group[PORTA].OUTSET.reg = (1 << PINA3);
      break;

      case(BLUE):
      // charlieplexing: set pins uninvolved with bottom layer being blue to INPUT
      PORT->Group[PORTA].DIRCLR.reg = (1 << PINA0);
      PORT->Group[PORTB].DIRCLR.reg = (1 << PINA1);
      PORT->Group[PORTB].DIRCLR.reg = (1 << PINA2);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PINA3);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN10);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN11);
      PORT->Group[PORTA].DIRCLR.reg = (1 << PIN12);
      PORT->Group[PORTB].DIRCLR.reg = (1 << PIN13); 

      // drive blue pins high
      PORT->Group[PORTA].DIRSET.reg = (1 << PIN2);
      PORT->Group[PORTA].DIRSET.reg = (1 << PIN3);
      PORT->Group[PORTB].DIRSET.reg = (1 << PIN4);
      PORT->Group[PORTB].DIRSET.reg = (1 << PIN5);
      PORT->Group[PORTA].OUTSET.reg = (1 << PIN2);
      PORT->Group[PORTA].OUTSET.reg = (1 << PIN3);
      PORT->Group[PORTB].OUTSET.reg = (1 << PIN4);
      PORT->Group[PORTB].OUTSET.reg = (1 << PIN5);
      break;
    default:
      break;
  }
  delay(delay_time);
}

void make_yellow(int delay_time) {
  bottom_layer_solid(RED, delay_time);
  bottom_layer_solid(GREEN, delay_time);
  second_layer_solid(RED, delay_time);
  second_layer_solid(GREEN, delay_time);
  third_layer_solid(RED, delay_time);
  third_layer_solid(GREEN, delay_time);
  top_layer_solid(RED, delay_time);
  top_layer_solid(GREEN, delay_time);
}

void make_white(int delay_time) {
  bottom_layer_solid(RED, delay_time);
  bottom_layer_solid(GREEN, delay_time);
  bottom_layer_solid(BLUE, delay_time);
  second_layer_solid(RED, delay_time);
  second_layer_solid(GREEN, delay_time);
  second_layer_solid(BLUE, delay_time);
  third_layer_solid(RED, delay_time);
  third_layer_solid(GREEN, delay_time);
  third_layer_solid(BLUE, delay_time);
  top_layer_solid(RED, delay_time);
  top_layer_solid(GREEN, delay_time);
  top_layer_solid(BLUE, delay_time);
}

void all_layers_solid(Color color, int delay_time) {
  top_layer_solid(color, delay_time);
  third_layer_solid(color, delay_time);
  second_layer_solid(color, delay_time);
  bottom_layer_solid(color, delay_time);
}

// use pwm signal to make bottom layer (dim) blue
// demonstrates what happens without charlieplexing -- other lights will also light up
void cloudy_pwm() {
  analogWrite(2, 200);
  analogWrite(3, 200);
  analogWrite(4, 200);
  analogWrite(5, 200);
}