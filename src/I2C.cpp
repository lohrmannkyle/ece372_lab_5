#include "I2C.h"
#include <avr/io.h>
#include <Arduino.h>

#define wait_for_completion while(!(TWCR & (1<<TWINT))); //acknowledge flag goes up
#define trigger_action TWCR = ((1<<TWINT)|(1<<TWEN)); //sets interrupt and enable bits to 1

void InitI2C(){
    //Serial.println("InitI2C");
    PRR0 &= ~(1 << PRTWI);  //Restart oscilator
  
// generate a 100kHz clock rate
// SCL freq = 16Mhz/(16 + 2(TWBR)*1)
// Table 24-7 Prescaler value is 1 so TWPS0 = 0 and TWPS1 = 0.
  TWSR &= ~((1 << TWPS1)|(1 << TWPS0));
  //TWSR |= (1 << TWPS0);
  TWBR = 0x0C; // bit rate generator = 400k  (TWBR = 12)

  TWCR = (1 << TWINT | 1 << TWEN); // enable two wire interface

}

void StartI2C_Trans(unsigned char Slave_Address){
    //Serial.println("StartI2C");
    TWCR = ((1 << TWEN) | (1 << TWINT) | (1 << TWSTA));
    wait_for_completion;
    //Serial.println(TWSR, HEX);
    TWDR = Slave_Address; 
    //Serial.println("StartI2C end");
}

void StopI2C_Trans(){
    //Serial.println("StopI2C");
    TWCR = ((1 << TWEN) | (1 << TWINT) | (1 <<TWSTO));
}

void Write(unsigned char Slave_Address, unsigned char Data){
    //Serial.println("WriteI2C");
    StartI2C_Trans((Slave_Address<<1)& 0xFE);
    //Serial.println(TWSR, HEX);
    trigger_action;
    wait_for_completion;
    TWDR = Data;
    trigger_action;
    wait_for_completion;
    //Serial.println("WriteI2C complete");
}

void Read_from(unsigned char Slave_Address, unsigned char MEM_ADDRESS){ //slave address needs to be 7 bit, 8 bit mem address
    //Serial.println("Read_from entered");
    //Serial.println(TWSR, HEX);
    //Serial.println(Slave_Address);
    //Serial.println(TWSR, HEX);
    Write(Slave_Address, MEM_ADDRESS);
    trigger_action; TWCR |= (1<<TWSTA);
    wait_for_completion;
    //StartI2C_Trans((Slave_Address<<1)|0xFE); // start write
        //Serial.println("Read_from 1st start fin");
        //trigger_action;
        //wait_for_completion;
       //sets data register to mem address 
        //TWDR = MEM_ADDRESS;
        //trigger_action;
        //wait_for_completion;
    StartI2C_Trans((Slave_Address<<1)| 0x01); //start read
       // Serial.println("Read_from 2nd start fin");
        trigger_action; TWCR |= (1<<TWEA); // sends ack 
        wait_for_completion; 
        trigger_action;
        wait_for_completion;
        TWCR = ((1<<TWSTO)|(1<<TWEN)|(1<<TWINT));

}

unsigned char Read_data(){
    //Serial.println("Read_data");
    unsigned char x;
        x = TWDR;
    return x;
}