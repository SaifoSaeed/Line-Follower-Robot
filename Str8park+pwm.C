//LDR IS HIGH WHEN DARK

// --- 1. SENSOR DEFINITIONS (Simple Masks) ---
#define LDR         0x01  //RA0 (0000 0001)

#define S_WALL      0x80  //RB7 (1000 0000)

#define SERVO       0x01  //RD0 (0000 0001)
#define BUZZER      0x02  //RD1 (0000 0010)
#define _LED_1      0x04  //RD2 (0000 0100)
#define _LED_2      0x08  //RD3 (0000 1000)
#define S_LEFT      0x40  //RD6 (0100 0000)
#define S_MID       0x20  //RD5 (0010 0000)
#define S_RIGHT     0x10  //RD4 (0001 0000)
#define S_LR        S_LEFT | S_RIGHT
#define S_PARK      0x80

// --- 2. MOTOR DIRECTION DEFINITIONS ---
// To go FWD: RC3=1, RC4=0.  (0000 1000 -> 0x08)
// To go REV: RC3=0, RC4=1.  (0001 0000 -> 0x10)
#define DIR_FWD      0x08
#define DIR_REV      0x10
#define DIR_STOP     0x00

// --- 3. SPEED SETTINGS (0 to 255) ---
// Adjust these based on how fast your robot is!
#define SPEED_FAST   125   // Base running speed
#define SPEED_SLOW   100   // Turning inner wheel speed
#define SPEED_VERY_SLOW   50   // Turning inner wheel speed
#define SPEED_STOP   0     // Stop
#define DIFF_13      33    // ~13% of 255
#define INIT_DELAY  3000


volatile unsigned int delay_tick = 0;
volatile unsigned int buzz_tick = 0;
volatile unsigned char DELAY_PASSED = 0;
volatile unsigned char FINISHED = 0;
volatile unsigned char BUZZING = 0;
volatile unsigned char ULTRA_ON = 0;

// FOR ULTRASONIC 
volatile unsigned int T1overflow;
volatile unsigned long T1counts;
volatile unsigned long Distance;

void interrupt();
void setup();
unsigned int ADC_Read(void);
void usDelay(unsigned int us);
void read_sonar();


void read_sonar(void){

    T1overflow=0;
    TMR1H=0;
    TMR1L=0;

    PORTB |= 0x04; //Trigger the ultrasonic sensor (RB2 connected to trigger)
    usDelay(10);   //keep trigger for 10uS
    PORTB &= 0xFB; //Remove trigger

    while(!(PORTB&0x02));

    T1CON=0x19; //TMR1 ON,  Fosc/4 (inc 1uS) with 1:2 prescaler (TMR1 overflow after 0xFFFF counts ==65536)==> 65.536ms
   
    while(PORTB&0x02);

    T1CON=0x18; //TMR1 OFF,  Fosc/4 (inc 1uS) with 1:1 prescaler (TMR1 overflow after 0xFFFF counts ==65536)==> 65.536ms
    T1counts=((TMR1H<<8)|TMR1L)+(T1overflow*65536);
    Distance=((T1counts*34)/(1000))/2;
}



void usDelay(unsigned int us) {
    unsigned int i;

    for(i = 0; i < us; i++) {
        asm nop; // Fine-tuning
    }
}

  unsigned int ADC_Read(){
    ADCON0 |=  0x04;
    while(ADCON0 & 0x04);
    return ((ADRESH << 8) | ADRESL);
  }

  void interrupt(){
    //We assume that only TMR0 can trigger interrupts for now.

    if (!DELAY_PASSED){
        delay_tick++;
        INTCON &= 0xA0;
        if ( delay_tick > INIT_DELAY ){
            PORTD |= _LED_1;
            PORTD |= _LED_2;
            DELAY_PASSED = ~DELAY_PASSED;
            INTCON &= 0xA0;
        }
    }

    if (BUZZING){
        buzz_tick++;
        INTCON &= 0xA0;
    }

    else if ( !(PORTD & S_LEFT) && !(PORTD & S_RIGHT) && !(PORTD & S_MID) && (PORTD & S_PARK)){
        CCPR2L = SPEED_STOP;
        CCPR1L = SPEED_STOP;
        FINISHED = 1;
        INTCON &= 0xA0;
    }
    
    else {
        INTCON &= 0xA0;
    }
  }

  void setup() {
      // --- GPIO CONFIG ---
      TRISA = 0x01;
      TRISC = 0x00;   // PORTC is all Output (Motors)
      TRISD = 0xF0;   // PORTD upper nibble is Input (Sensors)
      TRISB = 0X02;   // RB2 = Trigger (output) & RB1 = Echo (input)


      PORTA = PORTB = PORTC = PORTD = PORTE = 0x00;   // Everything OFF initially

      // TIMERS Configuration
      TMR0 = TMR1H = TMR1L = TMR2 = 0;
      OPTION_REG = 0x02;      //TMR0 1ms overflow
      INTCON = 0xA0;

      // TMR1 CONFIGURATION
      T1CON = 0x19; 
      T1overflow=0;
      T1counts=0;
      Distance=1000;
      TMR1H=0;
      TMR1L=0;

      PIE1 = 0x01;
      // PIR1

      PIE2 = 0x00;
      // PIR2

      // --- PWM CONFIGURATION (The "Hard" Way) ---
      // We use Timer2 for both CCP1 and CCP2

      // 1. Set PWM Period (Frequency)
      // PR2 = [Fosc / (4 * TMR2Prescale * Freq)] - 1
      // For ~2kHz at 8MHz:
      PR2 = 0xFF;

      // 2. Configure CCP Modules to PWM Mode
      // CCPxCON <3:0> bits set to 1100 (0x0C) for PWM
      CCP1CON = 0x0C; // Enable PWM on RC2
      CCP2CON = 0x0C; // Enable PWM on RC1

      // 3. Set Initial Duty Cycle to 0 (Stopped)
      CCPR1L = 0;
      CCPR2L = 0;

      // 4. Configure Timer2
      // T2CON: Prescaler 1:16, Timer ON
      // Bit 2 = 1 (TMR2ON), Bits 1-0 = 1x (Prescaler 16) -> 0000 0111 -> 0x07 (actually 0x04 is 1:1, let's use 1:4 for smoother motor)
      // Let's use T2CON = 0x04 (Timer2 On, Prescaler 1:1) for high freq,
      // Or T2CON = 0x06 (Timer2 On, Prescaler 1:16) for lower freq torque.
      T2CON = 0x06; // Turn on Timer2

      ADCON0 = 0x01; // ADON = 1 , fosc/4 , AN0
      ADCON1 = 0xAE; // right.jusified , ALL digital except AN0
  }

  void main() {
      unsigned int reading;
      unsigned int voltage;
      setup();

      //Initial delay.
      while (delay_tick < INIT_DELAY);

      //Direction -> FWD
      PORTC = (PORTC & 0xE7) | DIR_FWD;

      while(1) {
          reading = ADC_Read();

          //Scaled 10 times.
          voltage = (reading * 50) / 1023;

            if (ULTRA_ON == 0x03){
                 read_sonar();
        }
        
            if (Distance < 10){
               // CCPR2L = SPEED_STOP;
               // CCPR1L = SPEED_FAST - DIFF_13;
               PORTB |= 0X08;
            }
            else {
                PORTB &= 0xF7;
            }

        //Buzzer Scope.
        {
            //Beeping?
            if (voltage >= 35) {
                ULTRA_ON &= 0x01;
                ULTRA_ON |= 0x01;
                BUZZING = 1;
                while(buzz_tick < 500) {
                    PORTD |= BUZZER;
                }
                
                buzz_tick = 0;
                while(buzz_tick < 500) {
                    PORTD &= ~BUZZER;   
                }
                buzz_tick = 0;
            }

            else {
                ULTRA_ON |= 0x02;
                BUZZING = 0;
                buzz_tick = 0;
                PORTD &= ~BUZZER;
            }
        }
      }
  }