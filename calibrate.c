/*
 * Project: Motor Speed Calibration Test
 * MCU: PIC16F877A @ 8.0 MHz
 *
 * PURPOSE:
 * Runs a sequence of speed differences to help diagnose
 * which motor is naturally faster/slower.
 * * SEQUENCE:
 * 1. Equal Speeds (50% Duty)
 * 2. Left 5% Faster (Relative)
 * 3. Right 5% Faster (Relative)
 * 4. Left 10% Faster
 * 5. Right 10% Faster
 *
 * TIMING:
 * - Run Time: 4 Seconds
 * - Pause Time: 2 Seconds
 */

// --- PIN DEFINITIONS ---
#define DIR_FWD      0x08  // RC3 High, RC4 Low
#define DIR_STOP     0x00

// --- SPEED SETTINGS (0-255) ---
#define BASE_SPEED   128   // ~50% Duty Cycle (Safe testing speed)
#define DIFF_13      33    // ~13% of 255

// Function Prototypes
void setup();
void Custom_Delay_ms(unsigned int ms);
void run_test(unsigned char left_spd, unsigned char right_spd);

void main() {
    setup();

    // Initial Start Delay (2 Seconds)
    Custom_Delay_ms(2000);
    
    // --- Test new: Right faster (+13%) ---
    run_test(BASE_SPEED - DIFF_13, BASE_SPEED);
    
    // --- FINISHED ---
    // Stop motors and sleep forever
    PORTC = (PORTC & 0xE7) | DIR_STOP;
    CCPR1L = 0;
    CCPR2L = 0;

    while(1); // Infinite loop to stop the program
}

// --- HELPER FUNCTION TO RUN A TEST STEP ---
void run_test(unsigned char left_spd, unsigned char right_spd) {
    // 1. Set Speeds
    CCPR1L = left_spd;   // Left Motor (RC2)
    CCPR2L = right_spd;  // Right Motor (RC1)

    // 2. Set Direction FWD
    PORTC = (PORTC & 0xE7) | DIR_FWD;

    // 3. Run for 4 Seconds
    Custom_Delay_ms(4000);

    // 4. STOP Motors
    CCPR1L = 0;
    CCPR2L = 0;
    PORTC = (PORTC & 0xE7) | DIR_STOP;

    // 5. Wait 2 Seconds
    Custom_Delay_ms(2000);
}

// --- CUSTOM DELAY FUNCTION (No Libraries) ---
void Custom_Delay_ms(unsigned int ms_count) {
    unsigned int i;
    unsigned int j;

    // Calibrated for 8MHz Clock roughly
    // 1ms inner loop approx
    for (i = 0; i < ms_count; i++) {
        for (j = 0; j < 665; j++) {
            asm nop; // No operation (waste time)
        }
    }
}

// --- SETUP ROUTINE ---
void setup() {
    // GPIO Config
    TRISC = 0x00;   // PORTC Output
    PORTC = 0x00;   // Reset

    // PWM Config (Timer2)
    PR2 = 0xFF;     // Max Period
    CCP1CON = 0x0C; // PWM Mode for RC2 (Left)
    CCP2CON = 0x0C; // PWM Mode for RC1 (Right)

    // Start Stopped
    CCPR1L = 0;
    CCPR2L = 0;

    // Timer2 Config
    // Prescaler 1:16 (T2CON = 0x06) to run motors smoothly
    T2CON = 0x06;
}