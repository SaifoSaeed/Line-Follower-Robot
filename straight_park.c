    // #include "pinout.h"

    #define _SERVO       0x03
    #define _LED         0x08
    #define _S_LEFT      0x40
    #define _S_MID       0x20
    #define _S_RIGHT     0x10
    #define _S_PARK      0x80

    #define S_LEFT       _S_LEFT | ~_S_RIGHT | ~_S_PARK 
    #define S_RIGHT     ~_S_LEFT |  _S_RIGHT | ~_S_PARK 
    #define S_MID       ~_S_LEFT |  _S_MID | ~_S_RIGHT | ~_S_PARK

    #define S_INT       _S_LEFT | _S_MID | _S_RIGHT | ~_S_PARK
    #define S_WHITE    ~_S_LEFT | ~_S_MID | ~_S_RIGHT| ~_S_PARK
    #define S_BLACK     _S_LEFT | _S_MID | _S_RIGHT 

    #define BW_FWD      0x06

    #define INIT_DELAY  3000

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

        if (~DELAY_PASSED){
            tick++;
            if ( tick > INIT_DELAY ){
                PORTD |= _LED;
                DELAY_PASSED = ~DELAY_PASSED;
            }
        }

        //Follow line. Straight
        else if ( PORTD & S_MID ) {  //Ultrasonic ?! because we want to slow down...
            PWM_Wheels(10,10);
        }

        //Park.
        else if ( PORTD & S_BLACK ) {
            FINISH = 1;
            PWM_Wheels(10,0);
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

        return;
    }

    int main(){
        setup();
        while (tick < INIT_DELAY);
        PORTC = 0x08;
        while(1){
            if (FINISH){
                asm SLEEP;
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