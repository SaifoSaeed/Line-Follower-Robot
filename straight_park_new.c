    #define PARK_IDLE    0
    #define PARK_BUZZ_ON 1
    #define PARK_BUZZ_OFF 2
    #define PARK_ACTION  3

    #define TUNNEL_OUT  0x03
    #define BUMP        0x06

    //--- PINOUT MASKS ---
    #define LDR         0x01  //RA0 (0000 0001)

    #define ECHO        0x02  //RB1
    #define TRIG        0x04  //RB2
    #define S_WALL      0x80  //RB7 (1000 0000)

    #define SERVO       0x01  //RD0
    #define BUZZER      0x02  //RD1
    #define _LED_1      0x04  //RD2
    #define _LED_2      0x08  //RD3
    #define S_LEFT      0x40  //RD6
    #define S_MID       0x20  //RD5
    #define S_RIGHT     0x10  //RD4
    #define S_PARK      0x80  //RD7

    // --- 2. MOTOR DIRECTION DEFINITIONS ---
    #define DIR_FWD      0x08 //RC3=1, RC4=0
    #define DIR_REV      0x10 //RC3=0, RC4=1
    #define DIR_STOP     0x00

    // --- 3. SPEED SETTINGS (0 to 255) ---
    #define SPEED_FAST   110 //<< SPEED_SCALER
    #define SPEED_SLOW   90 //<< SPEED_SCALER
    #define DIFF         24  //<< SPEED_SCALER
    #define SPEED_STOP   0  

    #define INIT_DELAY  3000
    #define ULTRA_DELAY 2500

    unsigned char park_state = PARK_IDLE;
    unsigned int servo_timer = 0; // Tracks 20ms cycles
    volatile unsigned int delay_tick = 0;
    volatile unsigned int buzz_tick = 0;
    unsigned char DELAY_PASSED = 0;
    volatile unsigned char FINISHED = 0;
    volatile unsigned char ULTRA_ON = 0;
    // volatile unsigned char BUZZING = 0;

    volatile unsigned int timeout;
    volatile unsigned long t1_counts;
    volatile unsigned long distance = 999;
    volatile unsigned char sequence = 0;

    void interrupt();
    void setup();
    unsigned int ADC_Read(void);
    void Micro_Delay(unsigned int us);
    int Ultrasonic_Read(void);

    //Read from Ultrasonic sensor.
    int Ultrasonic_Read(){
        TMR1H = 0;
        TMR1L = 0;

        PORTB |= TRIG;  // Trigger HIGH
        Micro_Delay(10);    // 10 µs delay
        
        PORTB = PORTB & 0xFB; // Trigger LOW
        
        //Wait for Echo HIGH.
        timeout = 0;
        
        while ( !(PORTB & ECHO) ) {
            
            if ( (~PORTD & S_LEFT) | (~PORTD & S_RIGHT) | (~PORTD & S_MID) ) {
                ULTRA_ON = BUMP;
                INTCON |= 0x80;
                return 999;
            }

            else if(++timeout >= ULTRA_DELAY) {
                INTCON |= 0x80;
                return 999;
            }
        }

        T1CON |= 0x01;

        //Wait for Echo LOW (max 30 ms).
        timeout = 0;
        while (PORTB & ECHO) {
            if ( (~PORTD & S_LEFT) | (~PORTD & S_RIGHT) | (~PORTD & S_MID) ) {
                ULTRA_ON = BUMP;
                INTCON |= 0x80;
                return 999;
            }

            if(++timeout >= ULTRA_DELAY){
                T1CON &= 0xFE;
                INTCON |= 0x80;
                return 999;
            }
        }

        T1CON &= 0xFE;
        INTCON |= 0x80;

        t1_counts = ((unsigned long)TMR1H << 8) | TMR1L;
        return (t1_counts >> 6) + (t1_counts >> 9); //CHECK
    }

    //Induce micro-second delay (estimation).
    void Micro_Delay(unsigned int us) {
        unsigned int i;

        for(i = 0; i < us; i++) {
            asm nop;
        }
    }

    //Read from ADC's CH A0.
    unsigned int ADC_Read(){
        ADCON0 |=  0x04;
        while(ADCON0 & 0x04);
        return ((ADRESH << 8) | ADRESL);
    }

    //Only TM0 can interrupt.
    void interrupt() {

        if ( !DELAY_PASSED ) {
            delay_tick++;
            INTCON &= 0xFB;
            if ( delay_tick > INIT_DELAY ) {
                PORTD |= (_LED_1 | _LED_2);
                DELAY_PASSED = 1;
                INTCON &= 0xFB;
            }
        }

       // if (BUZZING) {
             buzz_tick++;
             INTCON &= 0xFB;
         
        // Black area routine...
        // else if ( (ULTRA_ON & BUMP) & (~PORTD & S_LEFT) & (~PORTD & S_RIGHT) & (~PORTD & S_MID) ){
        //     CCPR2L = SPEED_STOP;
        //     CCPR1L = SPEED_STOP;
        //     FINISHED = 1;
        //     INTCON &= 0xFB;  //=> should we re-enable interrupts?
        //     //Flag!!         => in main()
        // }

        else {
            INTCON &= 0xFB;
        }
    }

    void setup() {
        //I/O.
        TRISA = 0x01;
        TRISB = 0X02;   
        TRISC = 0x00;   
        TRISD = 0xF0;

        PORTA = PORTB = PORTC = PORTD = PORTE = 0x00;

        TMR0 = TMR1H = TMR1L = TMR2 = 0;
        OPTION_REG = 0x02; //1ms TMR0 delay.
        INTCON = 0xA0;

        PIE1 = 0x00;
        PIE2 = 0x00;

        PR2 = 0xFF;

        CCP1CON = 0x0C;
        CCP2CON = 0x0C;

        CCPR1L = 0;
        CCPR2L = 0;

        T1CON |= 0x10; //TMR1 x2 prescaler.
        T2CON = 0x06;  //TMR2 x16 prescaler.

        ADCON0 = 0x01; //ADC-CH0, ADC GO.
        ADCON1 = 0xAE; //RJ and AN0 analog only.
    }

    void main() {
        unsigned int reading;
        unsigned int voltage;
        setup();

        //Initial delay.
        while (delay_tick < INIT_DELAY);

        //Main event loop.
        while(1) {
            //AND with 0xE7, and then choose DIR.
            PORTC = (PORTC & 0xE7) | DIR_FWD;
            reading = ADC_Read(); //LDR read (for buzzer).
                voltage = (reading * 50) / 1023; //Scaled 10 times.

                if ( ULTRA_ON == TUNNEL_OUT ) {  
                    distance = Ultrasonic_Read();

                    if ( distance < 10 ) {
                        PORTC = (PORTC & 0xE7) | DIR_REV;
                        CCPR2L = SPEED_SLOW;
                        CCPR1L = SPEED_SLOW - DIFF;
                        //100ms delay.
                        Micro_Delay(50000);
                        Micro_Delay(50000);

                        if (sequence < 2) {
                            CCPR2L = SPEED_STOP;
                            CCPR1L = SPEED_SLOW - DIFF;
                            sequence++;
                            Micro_Delay(41000);
                            Micro_Delay(41000);
                        }

                        else if (sequence == 2) {
                            CCPR2L = SPEED_SLOW;
                            CCPR1L = SPEED_STOP;
                            sequence = 0;
                            Micro_Delay(45000);
                            Micro_Delay(45000);
                        }
                        
                    }

                    else if ( (distance >= 15) && (distance < 30) && (sequence < 2) ) {
                        CCPR2L = SPEED_STOP;
                        CCPR1L = SPEED_SLOW - DIFF;
                        sequence++;
                        Micro_Delay(41000);
                        Micro_Delay(41000);
                    }

                    else if ( (distance >= 15) && (distance < 30) && (sequence == 2) ) {
                        CCPR2L = SPEED_SLOW;
                        CCPR1L = SPEED_STOP;
                        sequence = 0;
                        Micro_Delay(45000);
                        Micro_Delay(45000);
                    }

                    //Straight.
                    else if ( distance >= 35 ) {
                        CCPR2L = SPEED_SLOW;
                        CCPR1L = SPEED_SLOW - DIFF;
                    }

                }
                    
                //Movement Scope.
                else {

                    if (ULTRA_ON == BUMP) {
                        // distance = Ultrasonic_Read();
                        //Go right.
                        if ( ((~PORTD & S_RIGHT) | (~PORTD & S_LEFT) | (~PORTD & S_MID)) == (S_LEFT | S_MID | S_RIGHT) ) {
                             
                            CCPR2L = SPEED_SLOW;
                            CCPR1L = SPEED_SLOW - DIFF;
                            Micro_Delay(50000);
                            Micro_Delay(50000);
                    
                            CCPR2L = SPEED_STOP;
                            CCPR1L = SPEED_FAST - DIFF;
                            
                            Micro_Delay(50000);
                            Micro_Delay(50000);

                            CCPR2L = SPEED_STOP;
                            CCPR1L = SPEED_STOP;
                            PORTD &= ~(_LED_1 | _LED_2);

                            asm sleep;
                        }

                        else if (~PORTD & S_LEFT) {
                            CCPR2L = SPEED_SLOW;
                            CCPR1L = SPEED_STOP;
                        }

                        //Go left.
                        else if (~PORTD & S_RIGHT) {
                            CCPR2L = SPEED_STOP;
                            CCPR1L = SPEED_SLOW - DIFF;
                        }

                        //Straight.
                        else {
                            CCPR2L = SPEED_SLOW;
                            CCPR1L = SPEED_SLOW - DIFF;
                        }
                    }
                    
                    else {
                        //Intersection => hard-coded left turn.
                        if ( ((~PORTD & S_RIGHT) | (~PORTD & S_LEFT)) == (S_RIGHT | S_LEFT) ) { //0101 0000

                            CCPR2L = 135;
                            CCPR1L = SPEED_STOP;
                            //100ms delay.
                            Micro_Delay(50000); 
                            
                        }

                        //Go right.
                        else if (~PORTD & S_LEFT) {
                            CCPR2L = SPEED_SLOW;
                            CCPR1L = SPEED_STOP;
                            
                            //50ms delay.
                            Micro_Delay(30000); 
                        }

                        //Go left.
                        else if (~PORTD & S_RIGHT) {
                            CCPR2L = SPEED_STOP;
                            CCPR1L = SPEED_SLOW - DIFF;

                            Micro_Delay(30000);   
                        }

                        //Straight.
                        else {
                            CCPR2L = SPEED_FAST;
                            CCPR1L = SPEED_FAST - DIFF;
                        }
                    }    
                }

                //Buzzer Scope.
                {
                    if (voltage >= 28) {
                        PORTD |= BUZZER;
                        ULTRA_ON &= 0x01; //Clear ultrasonic state.
                        ULTRA_ON |= 0x01; //Enter tunnel.
                    }

                    else if (voltage < 28) {
                        PORTD &= ~BUZZER;
                        ULTRA_ON |= 0x02; //Exit tunnel.
                    }

                    //Beeping?
                    // if (voltage >= 25) {
                    //     BUZZING = 1;
                    //     while(buzz_tick < 500) {
                    //         PORTD |= BUZZER;
                    //     }
                        
                    //     buzz_tick = 0;
                    //     while(buzz_tick < 500) {
                    //         PORTD &= ~BUZZER;   
                    //     }
                    //     buzz_tick = 0;
                    // }

                    // else {
                    //     BUZZING = 0;
                    //     buzz_tick = 0;
                    //     PORTD &= ~BUZZER;
                    // }

                    //Only when ULTRA_ON == 0x03 do we infer that
                    //it has entered then exited the tunnel.
                }
            }      
            
                
             
