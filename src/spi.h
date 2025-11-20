#ifndef SPI_H
#define SPI_h

#include <stdint.h>


void init_spi();
void init_matrix();
void write(uint8_t data);
void send_max(uint8_t addr, uint8_t data);
void set_smile();
void set_frown();

#endif