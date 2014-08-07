/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "system.h"
#include "define.h"

#endif

/******************************************************************************/
/* Extern global variables declarations                                       */
/******************************************************************************/



/* Refer to the device datasheet for information about available
oscillator configurations. */

void ConfigureOscillator(void)
{

    OSCCONbits.IDLEN=1; /* Iddle mode when SLEEP instruction*/
    OSCCONbits.IRCF=0;  /* don't care*/
    OSCCONbits.SCS=0;   /*Primary oscillator selected*/

}

/* ---------------   GPIO configurations ---------------*/

void ConfigureGPIO(void)
{

    ANCON0 = 0;             // disable analog inputs
    ANCON1 = 0;             // disable analog inputs


    TRISA = 0xF;           // input for CAN adress selection
    

    TRISB = 0b11001000;    // output for TX CAN and backlight output, and for keyboard
    LATBbits.LATB2=1;      // fix output TXCAN
    LATBbits.LATB5=1;      // fix backlight output

    LATBbits.LATB0=1;      // T1 output to 1
    LATBbits.LATB1=1;      // T2 output to 1
    LATBbits.LATB4=1;      // T3 output to 1

    TRISC = 0b11110000;    // outputs for contrast adjustment , and keyboard
    LATCbits.LATC2=0;      // fix contrast output to 0

    LATCbits.LATC0=1;      // T4 output to 1
    LATCbits.LATC1=1;      // T5 output to 1
    LATCbits.LATC3=1;      // T6 output to 1

    TRISD = 0b11000000;     /* output for leds and buzzer*/
    LATD=0;

    TRISE = 0b00000000;     // LCD control signals.
    LATE = 0x00;

}

void ConfigureInterrupts(void)
{
    //all interrupts are low priority, except CAN module

    RCONbits.IPEN = 1;           // enables priority logic for Interrupts.
            
    INTCON = 0b00100000;        // GIE interrupts disabled for the moment, TMRO OVF activated
    INTCON2 = 0b11111000;       // low priority for TMR0 overflow
    INTCON3 = 0b00000000;       // no external interrupt

    PIR1 = 0;                   //clear IT flags
    PIR2 = 0;
    PIR3 = 0;
    PIR4 = 0;
    PIR5 = 0;

    PIE1 = 0;          
    PIE2 = 0;
    PIE3 = 0;
    PIE4 = 0;
    PIE5 = 0b00000011;          // enable CAN RX interrupts.

    IPR1= 0 ;                   // low priority IT's
    IPR2= 0 ;                   // low priority IT's
    IPR3= 0 ;                   // low priority IT's
    IPR4= 0 ;                   // low priority IT's
    IPR5= 0b00000011 ;          // High priority for CAN RX.
   
    INTCONbits.GIE=1;           // general IT enable
    INTCONbits.PEIE=1;          // peripheral IT enable
}

void ConfigureTimers(void)
{
    // timer0 used for 10ms tick, in main application

    T0CONbits.T08BIT = 0;       //TMR0 in 16 bits mode
    T0CONbits.T0CS = 0;         // TMRO source is internal clock
    T0CONbits.PSA = 0;          // PSA assigned to TIMER0
    T0CONbits.T0PS=6;           // 1:128 prescaler

    TMR0H = TMR0H_INIT;
    TMR0L = TMR0L_INIT;         // preload for 10ms overflow (TMR0 = 1250)

    T0CONbits.TMR0ON = 1;       // enable TIMER0

    // timer 2: used for contrast and backlight configuration.

    T2CONbits.T2OUTPS = 0;        // 1/1 postscaler
    T2CONbits.T2CKPS = 0x2;       // 1/16 prescaler
    PR2 = 100;                    // period register for use with contrast and backlight

    // timer 4: used for buzzer output
    
    T4CONbits.T4OUTPS = 0;        // 1/1 postscaler
    T4CONbits.T4CKPS = 0x2;
    PR4=0xFF;                     // period register for use with buzzer

  
}

void ConfigureCCP(void)
{
    CCP2CON = 0x0F;         // CCP2 in PWM mode
    CCP3CON = 0;            // CCP3 disabled
    CCP4CON = 0;            // CCP4 disabled
    CCP5CON = 0X0F;         // CCP5 in PWM mode
    
    CCPTMRS= 0b00000001;    // CCP5 based on Tmr2 
                            // CCP2 based on tmr2
                            // ECCP1 based on timer 4

    // CCP 2 and 5 configuration

    CCPR2L = 0;             // duty cycle to 100% for contrast
    CCPR5L = 50;            // duty cycle to 50% for backlight

    T2CONbits.TMR2ON=1;     // timer 2 activation

    //ECCP1 configuration

    ECCP1CONbits.P1M = 0b10;        // half bridge output
    ECCP1CONbits.CCP1M = 0b0000;    // all outputs of ECCP are disabled for the moment
    
    CCPR1L=0x7F;

    T4CONbits.TMR4ON=1;


}

char GetCanAdress(void)
{
    return (PORTA & 0b00000111);
}

char SystemTest(void)
{
    return TRUE;
}