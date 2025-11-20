#include <Arduino.h>
#include <avr/io.h>

#include "timer.h"
#include "spi.h"
#include "I2C.h"

typedef enum {WAIT_PRESS, DEBOUNCE_PRESS, WAIT_RELEASE, DEBOUNCE_RELEASE} switch_state;
typedef enum {SMILEY, FROWNY} face_state;

volatile switch_state state = WAIT_PRESS;
volatile face_state face = SMILEY;

int main(){
  Serial.begin(9600);
  init_timer_1();
  init_timer_3();
  sei();
  init_spi();
  init_matrix();
  set_smile();
    
  // toggle exists to allow silencing the alarm but as soon as accelerometer is back within accepted axis values
  // it resets. So say you go smiley -> frowny (alarm activates) -> button (silences) -> smiley -> frowny (alarm reactivates)
  int toggle = 0;

  while(1){

    switch(state){
      case WAIT_PRESS: {
        break;
    }

    case DEBOUNCE_PRESS: {
        delay_ms(40);
        state = WAIT_RELEASE;

        if (face == FROWNY && toggle == 0) {
            toggle = 1;
            update_duty(0);
        }
        break;
    }

    case WAIT_RELEASE: {
        break;
    }

    case DEBOUNCE_RELEASE: {
        delay_ms(40);
        state = WAIT_PRESS;
        break;
    }
}

        
  if (face == SMILEY) {
      set_smile();
      toggle = 0;
  } else { 
      set_frown();
      if (toggle == 0){
          update_duty(.6);
      }
    }
    
  }
  return 0;
}

//button press inturrupt
ISR(PCINT2_vect){
    Serial.println("in ISR");
    if (state == WAIT_PRESS){
        state = DEBOUNCE_PRESS;
        face == FROWNY;
    } else if (state == WAIT_RELEASE) {
        state = DEBOUNCE_RELEASE;
    }

}