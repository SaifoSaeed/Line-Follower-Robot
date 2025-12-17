#ifndef P16F877_H
#define P16F877_H

#ifndef BIT_AT
// #define BIT_AT(base,bitno) sbit at ((base<<3)+bitno)
#endif

//
// Register addresses.
//
#define INDF        0x0000
#define TMR0        0x0001
#define PCL        0x0002
#define STATUS        0x0003
#define FSR        0x0004
#define PORTA        0x0005
#define PORTB        0x0006
#define PORTC        0x0007
#define PORTD        0x0008
#define PORTE        0x0009
#define PCLATH        0x000A
#define INTCON        0x000B
#define PIR1        0x000C
#define PIR2        0x000D
#define TMR1L        0x000E
#define TMR1H        0x000F
#define T1CON        0x0010
#define TMR2        0x0011
#define T2CON        0x0012
#define SSPBUF        0x0013
#define SSPCON        0x0014
#define CCPR1L        0x0015
#define CCPR1H        0x0016
#define CCP1CON        0x0017
#define RCSTA        0x0018
#define TXREG        0x0019
#define RCREG        0x001A
#define CCPR2L        0x001B
#define CCPR2H        0x001C
#define CCP2CON        0x001D
#define ADRESH        0x001E
#define ADCON0        0x001F
#define OPTION_REG        0x0081
#define TRISA        0x0085
#define TRISB        0x0086
#define TRISC        0x0087
#define TRISD        0x0088
#define TRISE        0x0089
#define PIE1        0x008C
#define PIE2        0x008D
#define PCON        0x008E
#define SSPCON2        0x0091
#define PR2        0x0092
#define SSPADD        0x0093
#define SSPSTAT        0x0094
#define TXSTA        0x0098
#define SPBRG        0x0099
#define ADRESL        0x009E
#define ADCON1        0x009F
#define EEDATA        0x010C
#define EEADR        0x010D
#define EEDATH        0x010E
#define EEADRH        0x010F
#define EECON1        0x018C
#define EECON2        0x018D

//
// Memory organization.
//
#pragma maxram 0x1FF

#pragma memmap 0x0020 0x006f RAM 0x000
#pragma memmap 0x0070 0x007f RAM 0x180
#pragma memmap 0x00a0 0x00ef RAM 0x000
#pragma memmap 0x0110 0x016f RAM 0x000
#pragma memmap 0x0190 0x01ef RAM 0x000

#pragma memmap INDF INDF SFR 0x000        // INDF
#pragma memmap TMR0 TMR0 SFR 0x000        // TMR0
#pragma memmap PCL PCL SFR 0x000        // PCL
#pragma memmap STATUS STATUS SFR 0x000        // STATUS
#pragma memmap FSR FSR SFR 0x000        // FSR
#pragma memmap PORTA PORTA SFR 0x000        // PORTA
#pragma memmap PORTB PORTB SFR 0x000        // PORTB
#pragma memmap PORTC PORTC SFR 0x000        // PORTC
#pragma memmap PORTD PORTD SFR 0x000        // PORTD
#pragma memmap PORTE PORTE SFR 0x000        // PORTE
#pragma memmap PCLATH PCLATH SFR 0x000        // PCLATH
#pragma memmap INTCON INTCON SFR 0x000        // INTCON
#pragma memmap PIR1 PIR1 SFR 0x000        // PIR1
#pragma memmap PIR2 PIR2 SFR 0x000        // PIR2
#pragma memmap TMR1L TMR1L SFR 0x000        // TMR1L
#pragma memmap TMR1H TMR1H SFR 0x000        // TMR1H
#pragma memmap T1CON T1CON SFR 0x000        // T1CON
#pragma memmap TMR2 TMR2 SFR 0x000        // TMR2
#pragma memmap T2CON T2CON SFR 0x000        // T2CON
#pragma memmap SSPBUF SSPBUF SFR 0x000        // SSPBUF
#pragma memmap SSPCON SSPCON SFR 0x000        // SSPCON
#pragma memmap CCPR1L CCPR1L SFR 0x000        // CCPR1L
#pragma memmap CCPR1H CCPR1H SFR 0x000        // CCPR1H
#pragma memmap CCP1CON CCP1CON SFR 0x000        // CCP1CON
#pragma memmap RCSTA RCSTA SFR 0x000        // RCSTA
#pragma memmap TXREG TXREG SFR 0x000        // TXREG
#pragma memmap RCREG RCREG SFR 0x000        // RCREG
#pragma memmap CCPR2L CCPR2L SFR 0x000        // CCPR2L
#pragma memmap CCPR2H CCPR2H SFR 0x000        // CCPR2H
#pragma memmap CCP2CON CCP2CON SFR 0x000        // CCP2CON
#pragma memmap ADRESH ADRESH SFR 0x000        // ADRESH
#pragma memmap ADCON0 ADCON0 SFR 0x000        // ADCON0
#pragma memmap OPTION_REG OPTION_REG SFR 0x000        // OPTION_REG
#pragma memmap TRISA TRISA SFR 0x000        // TRISA
#pragma memmap TRISB TRISB SFR 0x000        // TRISB
#pragma memmap TRISC TRISC SFR 0x000        // TRISC
#pragma memmap TRISD TRISD SFR 0x000        // TRISD
#pragma memmap TRISE TRISE SFR 0x000        // TRISE
#pragma memmap PIE1 PIE1 SFR 0x000        // PIE1
#pragma memmap PIE2 PIE2 SFR 0x000        // PIE2
#pragma memmap PCON PCON SFR 0x000        // PCON
#pragma memmap SSPCON2 SSPCON2 SFR 0x000        // SSPCON2
#pragma memmap PR2 PR2 SFR 0x000        // PR2
#pragma memmap SSPADD SSPADD SFR 0x000        // SSPADD
#pragma memmap SSPSTAT SSPSTAT SFR 0x000        // SSPSTAT
#pragma memmap TXSTA TXSTA SFR 0x000        // TXSTA
#pragma memmap SPBRG SPBRG SFR 0x000        // SPBRG
#pragma memmap ADRESL ADRESL SFR 0x000        // ADRESL
#pragma memmap ADCON1 ADCON1 SFR 0x000        // ADCON1
#pragma memmap EEDATA EEDATA SFR 0x000        // EEDATA
#pragma memmap EEADR EEADR SFR 0x000        // EEADR
#pragma memmap EEDATH EEDATH SFR 0x000        // EEDATH
#pragma memmap EEADRH EEADRH SFR 0x000        // EEADRH
#pragma memmap EECON1 EECON1 SFR 0x000        // EECON1
#pragma memmap EECON2 EECON2 SFR 0x000        // EECON2


//         LIST
// P16F877.INC  Standard Header File, Version 1.00    Microchip Technology, Inc.
//         NOLIST

// This header file defines configurations, registers, and other useful bits of
// information for the PIC16F877 microcontroller.  These names are taken to match 
// the data sheets as closely as possible.  

// Note that the processor must be selected before this file is 
// included.  The processor may be selected the following ways:

//       1. Command line switch:
//               C:\ MPASM MYFILE.ASM /PIC16F877
//       2. LIST directive in the source file
//               LIST   P=PIC16F877
//       3. Processor Type entry in the MPASM full-screen interface

//==========================================================================
//
//       Revision History
//
//==========================================================================

//Rev:   Date:    Reason:

//1.12   01/12/00 Changed some bit names, a register name, configuration bits
//                 to match datasheet (DS30292B)
//1.00   08/07/98 Initial Release

//==========================================================================
//
//       Verify Processor
//
//==========================================================================

//        IFNDEF __16F877
//            MESSG "Processor-header file mismatch.  Verify selected processor."
//         ENDIF

//==========================================================================
//
//       Register Definitions
//
//==========================================================================

#define W                    0x0000
#define F                    0x0001

//----- Register Files------------------------------------------------------
//----- STATUS Bits --------------------------------------------------------

// BIT_AT(STATUS,7)        IRP;
// BIT_AT(STATUS,6)        RP1;
// BIT_AT(STATUS,5)        RP0;
// BIT_AT(STATUS,4)        NOT_TO;
// BIT_AT(STATUS,3)        NOT_PD;
// BIT_AT(STATUS,2)        Z;
// BIT_AT(STATUS,1)        DC;
// BIT_AT(STATUS,0)        C;

// //----- INTCON Bits --------------------------------------------------------

// BIT_AT(INTCON,7)        GIE;
// BIT_AT(INTCON,6)        PEIE;
// BIT_AT(INTCON,5)        T0IE;
// BIT_AT(INTCON,4)        INTE;
// BIT_AT(INTCON,3)        RBIE;
// BIT_AT(INTCON,2)        T0IF;
// BIT_AT(INTCON,1)        INTF;
// BIT_AT(INTCON,0)        RBIF;

// //----- PIR1 Bits ----------------------------------------------------------

// BIT_AT(PIR1,7)        PSPIF;
// BIT_AT(PIR1,6)        ADIF;
// BIT_AT(PIR1,5)        RCIF;
// BIT_AT(PIR1,4)        TXIF;
// BIT_AT(PIR1,3)        SSPIF;
// BIT_AT(PIR1,2)        CCP1IF;
// BIT_AT(PIR1,1)        TMR2IF;
// BIT_AT(PIR1,0)        TMR1IF;

// //----- PIR2 Bits ----------------------------------------------------------

// BIT_AT(PIR2,4)        EEIF;
// BIT_AT(PIR2,3)        BCLIF;
// BIT_AT(PIR2,0)        CCP2IF;

// //----- T1CON Bits ---------------------------------------------------------

// BIT_AT(T1CON,5)        T1CKPS1;
// BIT_AT(T1CON,4)        T1CKPS0;
// BIT_AT(T1CON,3)        T1OSCEN;
// BIT_AT(T1CON,2)        NOT_T1SYNC;
// BIT_AT(T1CON,2)        T1INSYNC;    // Backward compatibility only
// BIT_AT(T1CON,2)        T1SYNC;
// BIT_AT(T1CON,1)        TMR1CS;
// BIT_AT(T1CON,0)        TMR1ON;

// //----- T2CON Bits ---------------------------------------------------------

// BIT_AT(T2CON,6)        TOUTPS3;
// BIT_AT(T2CON,5)        TOUTPS2;
// BIT_AT(T2CON,4)        TOUTPS1;
// BIT_AT(T2CON,3)        TOUTPS0;
// BIT_AT(T2CON,2)        TMR2ON;
// BIT_AT(T2CON,1)        T2CKPS1;
// BIT_AT(T2CON,0)        T2CKPS0;

// //----- SSPCON Bits --------------------------------------------------------

// BIT_AT(SSPCON,7)        WCOL;
// BIT_AT(SSPCON,6)        SSPOV;
// BIT_AT(SSPCON,5)        SSPEN;
// BIT_AT(SSPCON,4)        CKP;
// BIT_AT(SSPCON,3)        SSPM3;
// BIT_AT(SSPCON,2)        SSPM2;
// BIT_AT(SSPCON,1)        SSPM1;
// BIT_AT(SSPCON,0)        SSPM0;

// //----- CCP1CON Bits -------------------------------------------------------

// BIT_AT(CCP1CON,5)        CCP1X;
// BIT_AT(CCP1CON,4)        CCP1Y;
// BIT_AT(CCP1CON,3)        CCP1M3;
// BIT_AT(CCP1CON,2)        CCP1M2;
// BIT_AT(CCP1CON,1)        CCP1M1;
// BIT_AT(CCP1CON,0)        CCP1M0;

// //----- RCSTA Bits ---------------------------------------------------------

// BIT_AT(RCSTA,7)        SPEN;
// BIT_AT(RCSTA,6)        RX9;
// BIT_AT(RCSTA,6)        RC9;    // Backward compatibility only
// BIT_AT(RCSTA,6)        NOT_RC8;    // Backward compatibility only
// BIT_AT(RCSTA,6)        RC8_9;    // Backward compatibility only
// BIT_AT(RCSTA,5)        SREN;
// BIT_AT(RCSTA,4)        CREN;
// BIT_AT(RCSTA,3)        ADDEN;
// BIT_AT(RCSTA,2)        FERR;
// BIT_AT(RCSTA,1)        OERR;
// BIT_AT(RCSTA,0)        RX9D;
// BIT_AT(RCSTA,0)        RCD8;    // Backward compatibility only

// //----- CCP2CON Bits -------------------------------------------------------

// BIT_AT(CCP2CON,5)        CCP2X;
// BIT_AT(CCP2CON,4)        CCP2Y;
// BIT_AT(CCP2CON,3)        CCP2M3;
// BIT_AT(CCP2CON,2)        CCP2M2;
// BIT_AT(CCP2CON,1)        CCP2M1;
// BIT_AT(CCP2CON,0)        CCP2M0;

// //----- ADCON0 Bits --------------------------------------------------------

// BIT_AT(ADCON0,7)        ADCS1;
// BIT_AT(ADCON0,6)        ADCS0;
// BIT_AT(ADCON0,5)        CHS2;
// BIT_AT(ADCON0,4)        CHS1;
// BIT_AT(ADCON0,3)        CHS0;
// BIT_AT(ADCON0,2)        GO;
// BIT_AT(ADCON0,2)        NOT_DONE;
// BIT_AT(ADCON0,2)        GO_DONE;
// BIT_AT(ADCON0,0)        ADON;

// //----- OPTION_REG Bits -----------------------------------------------------

// BIT_AT(OPTION_REG,7)        NOT_RBPU;
// BIT_AT(OPTION_REG,6)        INTEDG;
// BIT_AT(OPTION_REG,5)        T0CS;
// BIT_AT(OPTION_REG,4)        T0SE;
// BIT_AT(OPTION_REG,3)        PSA;
// BIT_AT(OPTION_REG,2)        PS2;
// BIT_AT(OPTION_REG,1)        PS1;
// BIT_AT(OPTION_REG,0)        PS0;

// //----- TRISE Bits ---------------------------------------------------------

// BIT_AT(TRISE,7)        IBF;
// BIT_AT(TRISE,6)        OBF;
// BIT_AT(TRISE,5)        IBOV;
// BIT_AT(TRISE,4)        PSPMODE;
// BIT_AT(TRISE,2)        TRISE2;
// BIT_AT(TRISE,1)        TRISE1;
// BIT_AT(TRISE,0)        TRISE0;

// //----- PIE1 Bits ----------------------------------------------------------

// BIT_AT(PIE1,7)        PSPIE;
// BIT_AT(PIE1,6)        ADIE;
// BIT_AT(PIE1,5)        RCIE;
// BIT_AT(PIE1,4)        TXIE;
// BIT_AT(PIE1,3)        SSPIE;
// BIT_AT(PIE1,2)        CCP1IE;
// BIT_AT(PIE1,1)        TMR2IE;
// BIT_AT(PIE1,0)        TMR1IE;

// //----- PIE2 Bits ----------------------------------------------------------

// BIT_AT(PIE2,4)        EEIE;
// BIT_AT(PIE2,3)        BCLIE;
// BIT_AT(PIE2,0)        CCP2IE;

// //----- PCON Bits ----------------------------------------------------------

// BIT_AT(PCON,1)        NOT_POR;
// BIT_AT(PCON,0)        NOT_BO;
// BIT_AT(PCON,0)        NOT_BOR;

// //----- SSPCON2 Bits --------------------------------------------------------

// BIT_AT(SSPCON2,7)        GCEN;
// BIT_AT(SSPCON2,6)        ACKSTAT;
// BIT_AT(SSPCON2,5)        ACKDT;
// BIT_AT(SSPCON2,4)        ACKEN;
// BIT_AT(SSPCON2,3)        RCEN;
// BIT_AT(SSPCON2,2)        PEN;
// BIT_AT(SSPCON2,1)        RSEN;   
// BIT_AT(SSPCON2,0)        SEN;   

// //----- SSPSTAT Bits -------------------------------------------------------

// BIT_AT(SSPSTAT,7)        SMP;
// BIT_AT(SSPSTAT,6)        CKE;
// BIT_AT(SSPSTAT,5)        D;
// BIT_AT(SSPSTAT,5)        I2C_DATA;
// BIT_AT(SSPSTAT,5)        NOT_A;
// BIT_AT(SSPSTAT,5)        NOTESS;
// BIT_AT(SSPSTAT,5)        D_A;
// BIT_AT(SSPSTAT,5)        DATAESS;
// BIT_AT(SSPSTAT,4)        P;
// BIT_AT(SSPSTAT,4)        I2C_STOP;
// BIT_AT(SSPSTAT,3)        S;
// BIT_AT(SSPSTAT,3)        I2C_START;
// BIT_AT(SSPSTAT,2)        R;
// BIT_AT(SSPSTAT,2)        I2C_READ;
// BIT_AT(SSPSTAT,2)        NOT_W;
// BIT_AT(SSPSTAT,2)        NOT_WRITE;
// BIT_AT(SSPSTAT,2)        R_W;
// BIT_AT(SSPSTAT,2)        READ_WRITE;
// BIT_AT(SSPSTAT,1)        UA;
// BIT_AT(SSPSTAT,0)        BF;

// //----- TXSTA Bits ---------------------------------------------------------

// BIT_AT(TXSTA,7)        CSRC;
// BIT_AT(TXSTA,6)        TX9;
// BIT_AT(TXSTA,6)        NOT_TX8;    // Backward compatibility only
// BIT_AT(TXSTA,6)        TX8_9;    // Backward compatibility only
// BIT_AT(TXSTA,5)        TXEN;
// BIT_AT(TXSTA,4)        SYNC;
// BIT_AT(TXSTA,2)        BRGH;
// BIT_AT(TXSTA,1)        TRMT;
// BIT_AT(TXSTA,0)        TX9D;
// BIT_AT(TXSTA,0)        TXD8;    // Backward compatibility only

// //----- ADCON1 Bits --------------------------------------------------------

// BIT_AT(ADCON1,7)        ADFM;
// BIT_AT(ADCON1,3)        PCFG3;
// BIT_AT(ADCON1,2)        PCFG2;
// BIT_AT(ADCON1,1)        PCFG1;
// BIT_AT(ADCON1,0)        PCFG0;

// //----- EECON1 Bits --------------------------------------------------------

// BIT_AT(EECON1,7)        EEPGD;
// BIT_AT(EECON1,3)        WRERR;
// BIT_AT(EECON1,2)        WREN;
// BIT_AT(EECON1,1)        WR;
// BIT_AT(EECON1,0)        RD;

//==========================================================================
//
//       RAM Definition
//
//==========================================================================

//         __MAXRAM H'1FF'
//         __BADRAM H'8F'-H'90', H'95'-H'97', H'9A'-H'9D'
//         __BADRAM H'105', H'107'-H'109'
//         __BADRAM H'185', H'187'-H'189', H'18E'-H'18F'

//==========================================================================
//
//       Configuration Bits
//
//==========================================================================

#define _CP_ALL              0x0FCF
#define _CP_HALF             0x1FDF
#define _CP_UPPER_256        0x2FEF
#define _CP_OFF              0x3FFF
#define _DEBUG_ON            0x37FF
#define _DEBUG_OFF           0x3FFF
#define _WRT_ENABLE_ON       0x3FFF
#define _WRT_ENABLE_OFF      0x3DFF
#define _CPD_ON              0x3EFF
#define _CPD_OFF             0x3FFF
#define _LVP_ON              0x3FFF
#define _LVP_OFF             0x3F7F
#define _BODEN_ON            0x3FFF
#define _BODEN_OFF           0x3FBF
#define _PWRTE_OFF           0x3FFF
#define _PWRTE_ON            0x3FF7
#define _WDT_ON              0x3FFF
#define _WDT_OFF             0x3FFB
#define _LP_OSC              0x3FFC
#define _XT_OSC              0x3FFD
#define _HS_OSC              0x3FFE
#define _RC_OSC              0x3FFF

//         LIST
#endif

