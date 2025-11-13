#include <Arduino.h>
#include <avr/io.h>
#include "timer.h"
#include "cpi.h"
#include "I2C.h"
#include "pwm.h"
#include "switch.h"

//switch debounce statemachine
typedef enum {wait_press, pressed, wait_release, released} debounce;

//8x8 Display 
typedef enum {smiley, frowny} faceDisplay;

int main(){

    //initializations
    initTimer();
    initSwitchPin();
    initT4PWM();

    //set duty cycle
    setT4DutyCycle(.5);
    //Enable interrupts
    sei();

    /////////////////////////////////MAIN While Loop////////////////////////////////////
    while(1){
        //handle switch debouncing
    //Debounce state
        switch (debounce){
          case wait_press:
            timerDelay_ms(1);
          break;

          case pressed:
            timerDelay_ms(1);
            debounce = wait_release;
          break;

          case wait_release:
            timerDelay_ms(1);
          break;

          case released:
            timerDelay_ms(1);
            debounce = wait_press;
            //change logic here
            break;

    return 0;
}

//button press inturrupt
ISR (INT0_vect){//PCINT1_vect) {
  if(debounce == wait_press){
    debounce = pressed;
  }
  else if(debounce == wait_release){
      debounce = released;
  }
}