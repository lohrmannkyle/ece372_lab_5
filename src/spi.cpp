#include <avr/io.h>
#include "spi.h"


void init_spi(){

    // Set SPI pins as output, Enable SPI, Set as master, set clockrate
    DDRB |= (1 << PB2) | (1 << PB1) | (1 << PB0); 
    SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}


void init_matrix(){
    send_max(0x09, 0x00); // no decode
    send_max(0x0A, 0x03); // 3/15 brightness
    send_max(0x0B, 0x07); // using all rows
    send_max(0x0C, 0x01); // normal mode (not off)
    send_max(0x0F, 0x00); // test off
}

void write(uint8_t data){

    // wait for buffer to clear before updating
    while (!(SPSR & (1 << SPIF)));
    SPDR = data;
}

void send_max(uint8_t addr, uint8_t data){

    // Set SS low so MAX expects transmit
    PINB &= (1 << PB0);

    // send MSB followed by MSB
    write(addr);
    write(data);

    // Set SS high to latch
    PINB |= (1 << PB0);
}

void set_smile(){
    send_max(0, 00000000);
    send_max(1, 00100100);
    send_max(2, 00100100);
    send_max(3, 00000000);
    send_max(4, 01000010);
    send_max(5, 00100100);
    send_max(6, 00011000);
    send_max(7, 00000000);
}

void set_frown(){
    send_max(0, 00000000);
    send_max(1, 00100100);
    send_max(2, 00100100);
    send_max(3, 00000000);
    send_max(4, 00011000);
    send_max(5, 00100100);
    send_max(6, 01000010);
    send_max(7, 00000000);
}