#include <Arduino.h>
#include <avr/io.h>
#include "timer.h"

//switch debounce statemachine
typedef enum {wait_press, pressed, wait_release, released} debounce_state;

//8x8 Display 
typedef enum {smiley, frowny} faceDisplay;

int main(){

    //initializations
    initTimer();
    




    return 0;
}