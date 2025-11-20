#include "I2C.h"
#include <avr/io.h>

#define wait_for_completion while (!(TWCR & (1<<TWINT)); //acknowledge flag goes up

void InitI2C(){
PRR0 &= ~(1 << PRTWI);  //Restart oscilator
  
// generate a 100kHz clock rate
// SCL freq = 16Mhz/(16 + 2(TWBR)*1)
// Table 24-7 Prescaler value is 1 so TWPS0 = 1 and TWPS1 = 0.
  TWSR &= ~((1 << TWPS0)|(TWPS1));
  TWBR = 0x48; // bit rate generator = 100k  (TWBR = 42)

  TWCR |= (1 << TWINT | 1 << TWEN); // enable two wire interface

}

void StartI2C_Trans(unsigned char Slave_Address){
    TWCR = ((1 << TWEN) | (1 << TWINT) | (1 << TWSTA));
    wait_for_completion;
}

void StopI2C_Trans(){


}

void Write(unsigned char Data){


}

void Read_from(unsigned char Slave_Address, unsigned char MEM_ADDRESS){


}

unsigned char Read_data(){
    unsigned char x;


    return x;
}