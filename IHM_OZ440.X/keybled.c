
/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/
#if defined(__XC)
    #include <xc.h>        /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>       /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>   /* C18 General Include File */
#endif

#include "system.h"
#include "keybled.h"
#include "define.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/* variable declard in Main                                                   */
/******************************************************************************/

extern struct KeybMsg KeybMsg;
extern struct LedDisp LedDisp;

/******************************************************************************/

void LedDisplay()
{
    if(LedDisp.LedDiplayCycle==0)
    {
        TRISDbits.TRISD0=0;
        TRISDbits.TRISD1=1;
        TRISDbits.TRISD2=0;
        TRISDbits.TRISD3=1;
        
        if((LedDisp.LightedLeds & 0b00000001)==0b00000001)
        {
            if((LedDisp.LedColor & 0b00000001)==0b00000001)
            {
                LATDbits.LATD0=1;
                LATDbits.LATD2=0;
            }
            else
            {
                LATDbits.LATD0=0;
                LATDbits.LATD2=1;
            }
        }
        else
        {
            LATDbits.LATD0=0;
            LATDbits.LATD2=0;
        }

        LedDisp.LedDiplayCycle++;
    }

    else if(LedDisp.LedDiplayCycle==1)
    {

        TRISDbits.TRISD0=0;
        TRISDbits.TRISD1=1;
        TRISDbits.TRISD2=1;
        TRISDbits.TRISD3=0;

        if((LedDisp.LightedLeds & 0b00000010)==0b00000010)
        {
            if((LedDisp.LedColor & 0b00000010)==0b00000010)
            {
                LATDbits.LATD0=1;
                LATDbits.LATD3=0;
            }
            else
            {
                LATDbits.LATD0=0;
                LATDbits.LATD3=1;
            }
        }
        else
        {
            LATDbits.LATD0=0;
            LATDbits.LATD3=0;
        }

        LedDisp.LedDiplayCycle++;
    }

    else if(LedDisp.LedDiplayCycle==2)
    {
        TRISDbits.TRISD0=1;
        TRISDbits.TRISD1=0;
        TRISDbits.TRISD2=0;
        TRISDbits.TRISD3=1;

        if((LedDisp.LightedLeds & 0b00000100)==0b00000100)
        {
            if((LedDisp.LedColor & 0b00000100)==0b00000100)
            {
                LATDbits.LATD1=1;
                LATDbits.LATD2=0;
            }
            else
            {
                LATDbits.LATD1=0;
                LATDbits.LATD2=1;
            }
        }
        else
        {
            LATDbits.LATD1=0;
            LATDbits.LATD2=0;
        }

        LedDisp.LedDiplayCycle++;
    }

    else if(LedDisp.LedDiplayCycle==3)
    {
        TRISDbits.TRISD0=1;
        TRISDbits.TRISD1=0;
        TRISDbits.TRISD2=1;
        TRISDbits.TRISD3=0;

        if((LedDisp.LightedLeds & 0b00001000)==0b00001000)
        {
            if((LedDisp.LedColor & 0b00001000)==0b00001000)
            {
                LATDbits.LATD1=1;
                LATDbits.LATD3=0;
            }
            else
            {
                LATDbits.LATD1=0;
                LATDbits.LATD3=1;
            }
        }
        else
        {
            LATDbits.LATD1=0;
            LATDbits.LATD3=0;
        }

        LedDisp.LedDiplayCycle=0;
    }

}

void KeybConfiguration(char longpresstime,char firetime)
{

    KeybMsg.KeybLongPressDelay=longpresstime;
    KeybMsg.KeybFireDelay=firetime;
    
}

char KeybRead(void)
{
    char reading=0;

    TRISBbits.TRISB0=1;      // T1 to input
    TRISBbits.TRISB1=1;      // T2 to input
    TRISBbits.TRISB4=1;      // T3 to input
    TRISCbits.TRISC0=1;      // T4 to input
    TRISCbits.TRISC1=1;      // T5 to input
    TRISCbits.TRISC3=1;      // T6 to input

    for(char i=0;i<200;i++);             // wait for 20 cycles

    if(!PORTBbits.RB0)
        reading = reading | 0b00000001;
    if(!PORTBbits.RB1)
        reading = reading | 0b00000010;
    if(!PORTBbits.RB4)
        reading = reading | 0b00000100;
    if(!PORTCbits.RC0)
        reading = reading | 0b00001000;
    if(!PORTCbits.RC1)
        reading = reading | 0b00010000;
    if(!PORTCbits.RC3)
        reading = reading | 0b00100000;
    
    TRISBbits.TRISB0=0;      // T1 to output
    TRISBbits.TRISB1=0;      // T2 to output
    TRISBbits.TRISB4=0;      // T3 to output
    TRISCbits.TRISC0=0;      // T4 to output
    TRISCbits.TRISC1=0;      // T5 to output
    TRISCbits.TRISC3=0;      // T6 to output

    LATBbits.LATB0=1;       // T1 output to 1
    LATBbits.LATB1=1;       // T2 output to 1
    LATBbits.LATB4=1;       // T3 output to 1
    LATCbits.LATC0=1;       // T4 output to 1
    LATCbits.LATC1=1;       // T5 output to 1
    LATCbits.LATC3=1;       // T6 output to 1

    return reading;

}