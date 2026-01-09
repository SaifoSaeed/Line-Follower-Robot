//LDR IS HIGH WHEN DARK

// --- 1. SENSOR DEFINITIONS (Simple Masks) ---
#define LDR          0x01  // RA0 (0000 0001)
#define BUZZER       0x02  // RD1 (0000 0010)
#define _LED_1       0x04  // RD2 (0000 0100)
#define _LED_2       0x08  // RD3 (0000 1000)
#define S_LEFT_PIN   0x40  // RD6 (0100 0000)
#define S_MID_PIN    0x20  // RD5 (0010 0000)
#define S_RIGHT_PIN  0x10  // RD4 (0001 0000)

// --- 2. MOTOR DIRECTION DEFINITIONS ---
// To go FWD: RC3=1, RC4=0.  (0000 1000 -> 0x08)
// To go REV: RC3=0, RC4=1.  (0001 0000 -> 0x10)
#define DIR_FWD      0x08 
#define DIR_REV      0x10
#define DIR_STOP     0x00

// --- 3. SPEED SETTINGS (0 to 255) ---
// Adjust these based on how fast your robot is!
#define SPEED_FAST   150   // Base running speed
#define SPEED_SLOW   70 // Turning inner wheel speed
#define SPEED_STOP   0     // Stop
#define DIFF_13      33    // ~13% of 255

volatile unsigned int delay_tick = 0;
#define INIT_DELAY  3000
unsigned char DELAY_PASSED = 0;
unsigned char FINISHED = 0;

void interrupt();
void setup();
void PWM(unsigned int P, unsigned int D);
unsigned int ADC_Read(void);

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

    else {
        INTCON &= 0xA0;
    }
}

void setup() {
    // --- GPIO CONFIG ---
    TRISA = 0x01;
    TRISC = 0x00;   // PORTC is all Output (Motors)
    TRISD = 0xF0;   // PORTD upper nibble is Input (Sensors)
    
    PORTA = PORTB = PORTC = PORTD = PORTE = 0x00;   // Everything OFF initially

    // TIMERS Configuration 
    TMR0 = TMR1H = TMR1L = TMR2 = 0;   
    OPTION_REG = 0x02;      //TMR0 1ms overflow
    INTCON = 0xA0;

    // TMR1 CONFIGURATION
    T1CON = 0B00

    PIE1 = 0x00;
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

        {
            //Beeping?
            if (voltage >= 35) {
                PORTD |= BUZZER;
            }
            
            else {
                PORTD &= ~BUZZER;
            }
        }

        {
            if (~PORTD & S_LEFT_PIN) {
                CCPR2L = SPEED_SLOW;
                CCPR1L = SPEED_STOP;
            }
    
            else if (~PORTD & S_RIGHT_PIN) {
                CCPR2L = SPEED_STOP;
                CCPR1L = SPEED_SLOW - DIFF_13;
            }

            else if (~PORTD & S_MID_PIN) {
                CCPR2L = SPEED_FAST;
                CCPR1L = SPEED_FAST - DIFF_13;
            }
            
            else {
                CCPR2L = SPEED_STOP;
                CCPR1L = SPEED_STOP;
            }
        }

        if (finished){
            PORTD |= 0x01;  // High
            usDelay(1500);  // 1.5ms delay for 90 degrees
            PORTD  = 0x00;  // Low
            usDelay(18500); // 18.5ms delay (rest of 20ms)
            asm sleep; 
        }

    }
}