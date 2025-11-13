#include <Arduino.h>
#include <avr/io.h>
#include "timer.h"
#include "cpi.h"
#include "I2C.h"
#include "pwm.h"
#include "switch.h"

//switch debounce statemachine
typedef enum {wait_press, pressed, wait_release, released} debounce_state;

//8x8 Display 
typedef enum {smiley, frowny} faceDisplay;

int main(){

    //initializations
    initTimer();
    




    return 0;
}