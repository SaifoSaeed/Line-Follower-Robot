#include "pinout.h"

#define _SERVO       0x03
#define _LED         0x04
#define _S_LEFT      0x05
#define _S_MID       0x06
#define _S_RIGHT     0x07
#define _S_PARK      0x08

#define S_LEFT       _S_LEFT | ~_S_RIGHT | ~_S_PARK 
#define S_RIGHT     ~_S_LEFT |  _S_RIGHT | ~_S_PARK 
#define S_MID       ~_S_LEFT |  _S_MID | ~_S_RIGHT | ~_S_PARK

#define S_INT       _S_LEFT | _S_MID | _S_RIGHT | ~_S_PARK
#define S_WHITE    ~_S_LEFT | ~_S_MID | ~_S_RIGHT| ~_S_PARK
#define S_BLACK     _S_LEFT | _S_MID | _S_RIGHT |  _S_PARK


#define INIT_DELAY  3000

//Function Declarations.
void interrupt();
void setup();

bool DELAY_PASSED = false;
bool FINISHED = false;
volatile unsigned int tick = 0;

//Interrupt Service Routine.
void interrupt(){
    //We assume that only TMR0 can trigger interrupts for now.

    if (~DELAY_PASSED){
        tick++
        if ( tick > INIT_DELAY ){
            PORTD |= _LED;
            DELAY_PASSED = ~DELAY_PASSED;
        }
    }

    //Turn left.
    else if ( PORTD & S_LEFT ) {
        //PWM_L = 70% PWM_R
        while(~(PORTD & S_MID)){
            //PWM_L -= 5% PWM_R 
        }
    }

    //Turn right.
    else if ( PORTD & S_RIGHT ) {
        //PWM_R = 70% PWM_L
        while(~(PORTD & S_MID)){
            //PWM_R -= 5% PWM_L
        }
    }

    //Follow line. Straight
    else if ( PORTD & S_MID ) {  //Ultrasonic ?! because we want to slow down...
        //PWM_L = PWM_R = 100%
    }

    else if (Ultrasonic < minimum_distance) {   //Ultrasonic ?! Smooth the braking in the main loop...
        while (PWM_L > 20%){
            PWM_L -= 10% ABS_PWM
            PWM_R -= 10% ABS_PWM        //ABS_PWM: absolute theoretical max.
        }
        //PWM_L = PWM_R = 20%    //Give it time to brake.
    }

    //Decide at intersection.
    else if ( PORTD & S_INT ) {
        //PWM_L = PWM_R = 0%
        //Trigger search using servo.
    }

    //White area.
    else if ( PORTD & S_WHITE ) {
        //Ensure car is out tunnel
    }

    /*
    else if( bump ?! ){
        How to detect?
    }
    */

    //Park.
    else if ( PORTD & S_BLACK ) {
        FINISH = true;
        /*
            The MCU must stay stuck here at the end.
            Hard-code the path to park.
            Slow down -> go sharp right -> turn around body center -> stop.
        */
        /*
        Transition from 20% to 5%.
        while(~parked){
            Beep buzzer every 1 second.
            Some parking logic...
        }
        */
        PORTD &= ~_LED; //Turn off LEDs.
        //Activate flag.
    }

    if (~FINISH){
        INTCON &= 0xFD;  //Clear TMR0IF.
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

    TRISC = 0x00;
    TRISD = 0xF0;
    PORTA = PORTB = PORTC = PORTD = PORTE = 0x00;

    TMR0 = TMR1H = TMR1L = TMR2 = 0;   
    OPTION_REG = 0x02;      //TMR0 1ms overflow
    INTCON = 0xA0;
    
    PIE1 = 0x00;
    // PIR1
    
    PIE2 = 0x00;
    // PIR2

    T2CON = 0x6F; // 0.1ms per count
    CCP1CON = CCP2CON = 0x0C; // Enable PWM for both motors
    PR2 = 99; // SO 100*0.1ms = 10ms period
    CCPR1L = 49; // WILL BE CHANGED DUE TO MOTORS SHIT
    CCPR2L = 49; //

    return;
}

int main(){
    setup();
    while (tick < INIT_DELAY);
    while(1){
        if (FINISH){
            asm SLEEP;
        }

        else{
            
        }
    }
    

    //How to follow the line?!

    //Ultrasonic    ->  RA0 

    /*
    They must have the same direction!
        H-Bridge in1/in3 -> RC3  
        H-Bridge in2/in4 -> RC4
    */

    /*
    TMR2 controls CCP1 and CCP2 -> Necessitates that both wheels always move at the same speed.
    Another approach is to use TMR1 for Wheel1 and TMR2 for Wheel2 -> Limits us with only one TMR0 for program flow.
        Motor PWM Right (H-bridge EnableA)    ->  RC1
        Motor PWM Left  (H-bridge EnableB)    ->  RC2
    */    
    //servo motor for flag -> RD1
    //Servo Motor   ->  RD2
    //2 LEDs        ->  RD3 (we will try to put both LEDs in the same node ~100ohm)
    //4 IR sensors  ->  RD4-7
    return 0;
}