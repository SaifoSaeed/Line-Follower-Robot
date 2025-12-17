#include "pinout.h"

//Function Declarations.
void interrupt();

//Interrupt Service Routine.
void interrupt(){
    
    return;
}

int main(){

    //Ultrasonic    ->  RA0 )  
    //Motor PWM Right (H-bridge EnableA)    ->  RC1
    //Motor PWM Left  (H-bridge EnableB)    ->  RC2
    //H-Bridge in1/in3 -> RC3 
    //H-Bridge in2/in4 -> RC4
    //Servo Motor   ->  RD2
    //2 LEDs        ->  RD3 (we will try to put both LEDs in the same node ~100ohm)
    //4 IR sensors  ->  RD4-7
    //

    return 0;
}