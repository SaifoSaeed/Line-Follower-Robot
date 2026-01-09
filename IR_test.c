// #include "pinout.h"

#define _SERVO       0x03
#define _LED         0x08
#define _S_PARK      0x80
#define _S_LEFT      0x40
#define _S_MID       0x20
#define _S_RIGHT     0x10

#define S_LEFT      (_S_LEFT  & ~_S_RIGHT)
#define S_RIGHT     (~_S_LEFT &  _S_RIGHT)
#define S_MID       (_S_MID & ~_S_PARK & ~_S_LEFT & ~_S_RIGHT)

#define S_INT       (_S_LEFT & _S_MID & _S_RIGHT & ~_S_PARK)

#define S_WHITE     (~_S_LEFT & ~_S_MID & ~_S_RIGHT)

#define S_BLACK     (_S_LEFT & _S_MID & _S_RIGHT & _S_PARK)

#define BW_FWD      0x06

#define INIT_DELAY  3000

#define INIT_WL_SPEED   149
#define INIT_WR_SPEED   119

//Function Declarations.
void interrupt();
void setup();
void myDelay(unsigned int);

unsigned char DELAY_PASSED = 0;
unsigned char FINISH = 0;
volatile unsigned int tick = 0;

unsigned char period;
unsigned char duty;
unsigned int Mcntr;

//First part of period.
unsigned char WL_SPEED_0 = INIT_WL_SPEED;
unsigned char WR_SPEED_0 = 0;

unsigned char WL_SPEED_1 = 0;
unsigned char WR_SPEED_1 = INIT_WR_SPEED;

void PWM_Wheels(unsigned int p, unsigned int d){
    // period in milliseconds, d 1-100 %
    //This will have the PWM Signal Out on PORTB1-7, so do the necessary initializations in the main.
    period=p;//milliseconds
    duty=(d*p)/100;
    PORTC |= BW_FWD;
    myDelay(duty);
    PORTC &= ~BW_FWD;
    myDelay(period-duty);
}

void myDelay(unsigned int x){
    Mcntr=0;
    while(Mcntr<x);
}

//Interrupt Service Routine.
void interrupt(){
    //We assume that only TMR0 can trigger interrupts for now.
    Mcntr++;
    if (~DELAY_PASSED){
        tick++;
        if ( tick > INIT_DELAY ){
            PORTD |= _LED;
            DELAY_PASSED = ~DELAY_PASSED;
        }
    }

    //Follow line. Straight
    else if ( PORTD & S_MID ) {  //Ultrasonic ?! because we want to slow down...
        WL_SPEED_0 = INIT_WL_SPEED;
        WR_SPEED_0 = 0;

        WL_SPEED_1 = 0;
        WR_SPEED_1 = INIT_WR_SPEED;
        
    }

    //Park.
    else if (PORTD & S_BLACK) {
        FINISH = 1;

        WL_SPEED_0 = 0;
        WR_SPEED_0 = 0;

        WL_SPEED_1 = 0;
        WR_SPEED_1 = 0;

        PORTD &= ~_LED; //Turn off LEDs.
        //Activate flag.
    }

    else if (PORTD & S_WHITE){
        FINISH = 1;

        WL_SPEED_0 = 0;
        WR_SPEED_0 = 0;

        WL_SPEED_1 = 0;
        WR_SPEED_1 = 0;

        PORTD &= ~_LED; //Turn off LEDs.
        //Activate flag.
    }

    if (~FINISH){
        INTCON &= 0xFB;  //Clear TMR0IF.
    }

    else {
        INTCON &= 0x80; 
    }

    return;
}

//Initialize Registers.
void setup(){

    // TRISA = 0x01; //Ultrasonic
    // ADCON0 = 0x01;
    // ADCON0 = 0x01;
    // ADCON1 = 0x0E;
    
    TRISB = 0x00;
    TRISC = 0x00; // 1111 0000
    TRISD = 0xF0;

    // CHECKING IR sensors
    
    PORTA = PORTB = PORTC = PORTD = PORTE = 0x00;

    TMR0 = TMR1H = TMR1L = TMR2 = 0;
    OPTION_REG = 0x02;      //TMR0 1ms overflow
    INTCON = 0xA0;
    
    PIE1 = 0x00;
    PIE2 = 0x00;

    return;
}

#define LED_TEST_L   0x01  // RB0
#define LED_TEST_M   0x02  // RB1
#define LED_TEST_R   0x04  // RB2
#define LED_TEST_P   0x08  // RB3

void main(){
    setup();

    // Set PORTB as Output for LEDs
    TRISB = 0x00; 
    PORTB = 0x00; // All OFF initially

    while(1){
        
        if (PORTD & _S_LEFT) {
            PORTB = PORTB | LED_TEST_L; 
        }
        else {
            PORTB = PORTB & ~LED_TEST_L; // 0xFE
        }

        // TEST 2: Check MID Sensor (RD5 / 0x20)
        if (PORTD & _S_MID) {
            PORTB = PORTB | LED_TEST_M;
        }
        else {
            PORTB = PORTB & ~LED_TEST_M; // 0xFD
        }

        // TEST 3: Check RIGHT Sensor (RD4 / 0x10)
        if (PORTD & _S_RIGHT) {
            PORTB = PORTB | LED_TEST_R;
        }
        else {
            PORTB = PORTB & ~LED_TEST_R; // 0xFB
        }

        // TEST 4: Check PARK Sensor (RD7 / 0x80)
        if (PORTD & _S_PARK) {
            PORTB = PORTB | LED_TEST_P;
        }
        else {
            PORTB = PORTB & ~LED_TEST_P; // 0xF7
        }
    }

    return;
}