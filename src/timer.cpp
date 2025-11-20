#include <avr/io.h>
#include <timer.h>

void initTimer(){
//Timer 0
    //Set timer mode bits - normal mode w/ prescaler (64)
        TCCR0A &= ~(1<<WGM00);
        TCCR0A &= ~(1<<WGM01);
        TCCR0B &= ~(1<<WGM02);

    //Setting timer 0 active with 64 prescaler, 1ms at 250 count
        TCCR0B |= (1<<CS00);
        TCCR0B |= (1<<CS01);
        TCCR0B &= ~(1<<CS02);

    // Set count reg to start
        TCNT0 = 5; //start value of 5(?), with 64x prescaler 250 count = 1ms 

    //reset flag value
        TIFR0 |= (1<<TOV0);

}


void timerDelay_ms(unsigned int msDelay){
    //reset timer to initial value
        TCNT0 = 5;
    //reset flag value
        TIFR0 |= (1<<TOV0);

    //Counter var to count number of timer delays
        unsigned int counter = 0;

     //loop
        while (counter < msDelay){
            //Serial.print(counter);
            if (TIFR0 & (1 << TOV0)){
                //reset flag
                TIFR0 |= (1 << TOV0);
                counter++;
            }
        }  
}





// KYLES

void init_timer_1(){
    // set to CTC (0100)
    TCCR1B |= (1 << WGM12);
    TCCR1B &= ~((1 << WGM13));
    TCCR1A &= ~((1 << WGM10) | (1 << WGM11));
}

//pwm
void init_timer_3(){
    DDRE |= (1 << PE3);
    
    // set pwm mode
    TCCR3A &= ~((1 << COM3A1) | (1 << COM3A0));
    TCCR3A |= (1 << COM3A1);
    TCCR3A &= ~(1 << COM3A0);

    // mode 14 (ICR3 as top) (1110)
    TCCR3B |= (1 << WGM33) | (1 << WGM32);
    TCCR3A |= (1 << WGM31);
    TCCR3A &= ~(1 << WGM30);

    // prescalar = 1
    TCCR3B |= (1 << CS30);
    TCCR3B &= ~((1 << CS31) | (1 << CS32));

    // top, sets f to about 16k, init duty cycle is 60%
    ICR3 = 1000;
    OCR3A = 600;

}

void update_duty(double duty){
    if (duty < 0 || duty >= (ICR3 / double(OCR3A)))
    OCR3A = int(duty * ICR3);
}


void chirp(){
    for (int i = 1000; i <= 4000; i += 100){
        ICR3 = i;
        update_duty(.6);
        delay_ms(5);
    }
    ICR3 = 1000;
    update_duty(.6);
}

void delay_ms(int delay){
    unsigned count = 0;

    // set prescalar to 64 (011)
    TCCR1B |= (1 << CS11) | (1 << CS10);
    TCCR1B &= ~(1 << CS12);

    // set top and start values
    TCNT1 = 0;
    OCR1A = 249;

    // ensure flag is cleared to begin
    TIFR1 = (1 << OCF1A);

    while (count < delay){
        while (!(TIFR1 & (1 <<OCF1A)));
        count++;
        TIFR1 = (1 << OCF1A);

    }

    // disable clock
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}

bool alarm_off(){
    return OCR3A == 0;
}