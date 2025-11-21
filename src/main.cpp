#include <Arduino.h>
#include <avr/io.h>

#include "timer.h"
#include "spi.h"
#include "I2C.h"
#include "switch.h"

typedef enum {WAIT_PRESS, DEBOUNCE_PRESS, WAIT_RELEASE, DEBOUNCE_RELEASE} switch_state;
typedef enum {SMILEY, FROWNY} face_state;

volatile switch_state state = WAIT_PRESS;
volatile face_state face = SMILEY;

int main(){
  Serial.begin(9600);
  init_timer_1();
  init_timer_3();
  init_switch();
  sei();
  init_spi();
  init_matrix();
  InitI2C();
  set_smile();
    
  // toggle exists to allow silencing the alarm but as soon as accelerometer is back within accepted axis values
  // it resets. So say you go smiley -> frowny (alarm activates) -> button (silences) -> smiley -> frowny (alarm reactivates)
  int toggle = 0;
  Write(0x6B, 0x00);
  StopI2C_Trans();
  while(1){
    
    Read_from(0x25, 0x3B); //X_high
    StopI2C_Trans();
    unsigned char XHIGH = Read_data();

    Read_from(0x25, 0x3C); //X_Low
    StopI2C_Trans();
    unsigned char XLOW = Read_data();

    unsigned char X_data = (XHIGH & 0xF0) | (XLOW & 0X0F);
    Serial.print("XHIGH: ");
    Serial.println(XHIGH);
    Serial.print("XLOW: ");
    Serial.println(XLOW);
    Serial.print("XDATA: ");
    Serial.println(X_data);

    StopI2C_Trans();
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

  chirp();
  delay_ms(500);

        
  if (face == SMILEY) {
      set_smile();
      toggle = 0;
  } else { 
      set_frown();
      if (toggle == 0){
          chirp();
      }
    }
    
  }
  return 0;
}

//button press inturrupt
ISR(PCINT2_vect){
  Serial.println("In interrupt");
    if (state == WAIT_PRESS){
        state = DEBOUNCE_PRESS;
    } else if (state == WAIT_RELEASE) {
        state = DEBOUNCE_RELEASE;
    }

}