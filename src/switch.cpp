#include "switch.h"
#include <avr/io.h>

/*
 * Initializes pull-up resistor on PB3 and sets it into input mode
 */
void initSwitchPin(){
    DDRB &= ~(1 << DDB3);
    PORTB |= (1 << PORTB3);
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << PCINT3);
}



// kyles

void init_switch(){
    // enable interrupts on pin A8 (PCINT16)

    DDRK &= ~(1 << DDK0);
    PORTK |= (1 << PORTK0);
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT16);
}
