#include <Arduino.h>
#include <avr/io.h>
#include <math.h>

#include "timer.h"
#include "spi.h"
#include "I2C.h"
#include "switch.h"

typedef enum {WAIT_PRESS, DEBOUNCE_PRESS, WAIT_RELEASE, DEBOUNCE_RELEASE} switch_state;
typedef enum {SMILEY, FROWNY} face_state;

volatile switch_state state = WAIT_PRESS;
volatile face_state face = SMILEY;

int main(){
  sei();
  Serial.begin(9600);
  init_timer_1();
  init_timer_3();
  init_switch();
  init_spi();
  init_matrix();
  InitI2C();
  set_smile();

    
  // toggle exists to allow silencing the alarm but as soon as accelerometer is back within accepted axis values
  // it resets. So say you go smiley -> frowny (alarm activates) -> button (silences) -> smiley -> frowny (alarm reactivates)
  int toggle = 0;

  Write(0b1101000, 0x6B);
  Write(0b1101000, 0x00);
  StopI2C_Trans();
  delay_ms(1);


  while(1){

    //XDATA
    //Serial.println(TWSR, HEX);
    Read_from(0b1101000, 0x3B); //X_high
    //Serial.println("Here");
    
    //StopI2C_Trans();
    signed char XHIGH = Read_data();
    //Serial.println(XHIGH);
    Read_from(0b1101000, 0x3C); //X_Low
    //Serial.println("Here2");
    
    StopI2C_Trans();
    //Serial.println(TWSR, HEX);
    signed char XLOW = Read_data();
    //StopI2C_Trans();
    //Serial.println(TWSR, HEX);

    signed int X_data = (XHIGH & 0xF0) | (XLOW & 0X0F);
    //Serial.print("XHIGH: ");
    //Serial.println(XHIGH);
    //Serial.print("XLOW: ");
    //Serial.println(XLOW);
    Serial.print("XDATA: ");
    Serial.println(X_data);

    //YDATA
    //Serial.println(TWSR, HEX);
    Read_from(0b1101000, 0x3D); //Y_high
    //Serial.println("Here");
    //StopI2C_Trans();
    signed char YHIGH = Read_data();
    Serial.println("Pre-YLow");
    Read_from(0b1101000, 0x3E); //Y_Low
    Serial.println("Here2");
    
    StopI2C_Trans();
    signed char YLOW = Read_data();
    //StopI2C_Trans();
    //Serial.println(TWSR, HEX);

    signed int Y_data = (YHIGH & 0xF0) | (YLOW & 0X0F);
    //Serial.print("XHIGH: ");
    //Serial.println(XHIGH);
    //Serial.print("XLOW: ");
    //Serial.println(XLOW);
    Serial.print("YDATA: ");
    Serial.println(Y_data);

    //ZDATA
    //Serial.println(TWSR, HEX);
    Read_from(0b1101000, 0x3F); //Z_high
    //Serial.println("Here");
    
    //StopI2C_Trans();
    signed char ZHIGH = Read_data();
    //Serial.println(YHIGH);
    Read_from(0b1101000, 0x40); //Z_Low
    //Serial.println("Here2");
    
    //StopI2C_Trans();
    //Serial.println(TWSR, HEX);
    signed char ZLOW = Read_data();
    StopI2C_Trans();
    //Serial.println(TWSR, HEX);

    signed int Z_data = (ZHIGH & 0xF0) | (ZLOW & 0X0F);
    //Serial.print("XHIGH: ");
    //Serial.println(XHIGH);
    //Serial.print("XLOW: ");
    //Serial.println(XLOW);
    Serial.print("ZDATA: ");
    Serial.println(Z_data);

    //Z-angles

    //README: add control logic for x and y close to 0 later so divergences are dealt with
   
    float zero_tolerance = 1e-6;
    if ((fabs(X_data) <= zero_tolerance) && (fabs(Y_data) <= zero_tolerance)){
      Serial.println("both angles within tolerance");
    }
    else{
      Serial.println("one or more angles not within tolerance");
    }
    /** 
    float theta = atan(Z_data/X_data);
    float phi = atan(Z_data/Y_data);

    float tolerance = 1e-6; //change this to whatever we experimentally verify
    if (theta <= tolerance){
      Serial.println('within tolerance, theta');
    }
    else{
      Serial.println('not within tolerance,theta');
    }

    if (phi <= tolerance){
      Serial.println('within tolerance, phi');
    }
    else{
      Serial.println('not within tolerance, phi');
    }
    **/
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

  //chirp();
  //delay_ms(500);

  
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