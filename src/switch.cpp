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
    // enable interrupts on pin 0 (PCINT8)

    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT8);
}
