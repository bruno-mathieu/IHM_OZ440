
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
#include "LCD.h"
#include "define.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/* variable declard in Main                                                   */
/******************************************************************************/



/******************************************************************************/


void LCDInit(void)
{
    __delay_ms(10);             // wait 40ms at startup
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);

    TRISCbits.TRISC4=0;         // LCD data lines in output
    TRISCbits.TRISC5=0;
    TRISCbits.TRISC6=0;
    TRISCbits.TRISC7=0;

    LCD_RS = 0;                 // instruction register
    LCD_RW = 0;                 // write to LCD

    LCD_EN = 1;                         // enable signal to 1
    LATC = (PORTC & 0x0F) | 0x30;      // instruction 0x3 for LCD (4 bits)
    for(char i=0;i<20;i++);             // wait for 20 cycles
    LCD_EN = 0;                         // enable signal to 0

    __delay_ms(5);                      // wait for 5ms

    LCD_EN = 1;                         // enable signal to 1
    LATC = (PORTC & 0x0F) | 0x30;      // instruction 0x3 for LCD (4 bits)
    for(char i=0;i<20;i++);             // wait for 20 cycles
    LCD_EN = 0;                         // enable signal to 0

    __delay_ms(5);                      // wait for 1ms

    LCD_EN = 1;                         // enable signal to 1
    LATC = (PORTC & 0x0F) | 0x30;      // instruction 0x3 for LCD (4 bits)
    for(char i=0;i<20;i++);             // wait for 20 cycles
    LCD_EN = 0;                         // enable signal to 0

     __delay_ms(5);                      // wait for 1ms

    //*********************** function set************************

    LCD_EN = 1;                         // enable signal to 1
    LATC = (PORTC & 0x0F) | 0x20;       // instruction 0x2 for LCD (4 bits)
    for(char i=0;i<20;i++);             // wait for 20 cycles
    LCD_EN = 0;                         // enable signal to 0
    
    __delay_ms(5);                      // wait for 1ms

     //*********** set to 4 bits, line number and font selection ********

    LCD_EN = 1;                         // enable signal to 1
    LATC = (PORTC & 0x0F) | 0x20;      // instruction 0x2 for LCD (4 bits)
    for(char i=0;i<20;i++);             // wait for 20 cycles
    LCD_EN = 0;                         // enable signal to 0

    __delay_ms(5);                      // wait for 1ms

    LCD_EN = 1;                         // enable signal to 1
    LATC = (PORTC & 0x0F) | 0xC0;      // instruction 0xC for LCD (4 bits)
    for(char i=0;i<20;i++);             // wait for 20 cycles
    LCD_EN = 0;                         // enable signal to 0

    __delay_ms(5);                      // wait for 1ms

    //*********************** Display off **********************

     LCD_EN = 1;                         // enable signal to 1
    LATC = (PORTC & 0x0F) | 0x00;      // instruction 0x0 for LCD (4 bits)
    for(char i=0;i<20;i++);             // wait for 20 cycles
    LCD_EN = 0;                         // enable signal to 0

    __delay_ms(5);                      // wait for 1ms

    LCD_EN = 1;                         // enable signal to 1
    LATC = (PORTC & 0x0F) | 0x80;      // instruction 0x8 for LCD (4 bits)
    for(char i=0;i<20;i++);             // wait for 20 cycles
    LCD_EN = 0;                         // enable signal to 0

    __delay_ms(5);                      // wait for 1ms

    //*********************** Display Clear **********************

    LCD_EN = 1;                         // enable signal to 1
    LATC = (PORTC & 0x0F) | 0x00;      // instruction 0x0 for LCD (4 bits)
    for(char i=0;i<20;i++);             // wait for 20 cycles
    LCD_EN = 0;                         // enable signal to 0

    __delay_ms(5);                      // wait for 1ms

    LCD_EN = 1;                         // enable signal to 1
    LATC = (PORTC & 0x0F) | 0x10;      // instruction 0x8 for LCD (4 bits)
    for(char i=0;i<20;i++);             // wait for 20 cycles
    LCD_EN = 0;                         // enable signal to 0

    __delay_ms(5);                      // wait for 1ms

    //*********************** Entry mode set: increment, no display shift **********************

    LCD_EN = 1;                         // enable signal to 1
    LATC = (PORTC & 0x0F) | 0x00;      // instruction 0x0 for LCD (4 bits)
    for(char i=0;i<20;i++);             // wait for 20 cycles
    LCD_EN = 0;                         // enable signal to 0

    __delay_ms(5);                      // wait for 1ms

    LCD_EN = 1;                         // enable signal to 1
    LATC = (PORTC & 0x0F) | 0x60;      // instruction 0x6 for LCD (4 bits)
    for(char i=0;i<20;i++);             // wait for 20 cycles
    LCD_EN = 0;                         // enable signal to 0

    __delay_ms(5);                      // wait for 1ms

    //*********************** Display ON, no cursor **********************

    LCD_EN = 1;                        // enable signal to 1
    LATC = (PORTC & 0x0F) | 0x00;       // instruction 0x0 for LCD (4 bits)
    for(char i=0;i<20;i++);             // wait for 20 cycles
    LCD_EN = 0;                         // enable signal to 0

    __delay_ms(5);                      // wait for 1ms

    LCD_EN = 1;                         // enable signal to 1
    LATC = (PORTC & 0x0F) | 0xC0;      // instruction 0xF for LCD (4 bits)
    for(char i=0;i<20;i++);             // wait for 20 cycles
    LCD_EN = 0;                         // enable signal to 0

    __delay_ms(5);                      // wait for 1ms


}

void LCDClearDisplay(void)
{

    TRISCbits.TRISC4=0;         // LCD data lines in output
    TRISCbits.TRISC5=0;
    TRISCbits.TRISC6=0;
    TRISCbits.TRISC7=0;

    LCD_RS = 0;                 // instruction register
    LCD_RW = 0;                 // write to LCD

    LCD_EN = 1;                         // enable signal to 1
    LATC = (PORTC & 0x0F) | 0x00;      // instruction 0x0 for LCD (4 bits)
    for(char i=0;i<20;i++);             // wait for 20 cycles
    LCD_EN = 0;                         // enable signal to 0

    for(char i=0;i<20;i++);             // wait for 20 cycles

    LCD_EN = 1;                         // enable signal to 1
    LATC = (PORTC & 0x0F) | 0x10;      // instruction 0x8 for LCD (4 bits)
    for(char i=0;i<20;i++);             // wait for 20 cycles
    LCD_EN = 0;                         // enable signal to 0

}

void LCDSendCharacter(char adress,char data)
{

    TRISCbits.TRISC4=0;         // LCD data lines in output
    TRISCbits.TRISC5=0;
    TRISCbits.TRISC6=0;
    TRISCbits.TRISC7=0;


    //************** write adress to  display ****************

    LCD_RS = 0;                                 // instruction register
    LCD_RW = 0;                                 // write to LCD

    adress = adress | 0x80;                     // set DB7 to 1

    LCD_EN = 1;                                 // enable signal to 1
    LATC = (PORTC & 0x0F) | (adress & 0xF0);    // instruction LCD (upper 4 bits
    for(char i=0;i<20;i++);                     // wait for 20 cycles
    LCD_EN = 0;                                 // enable signal to 0

    for(char i=0;i<20;i++);                     // wait for 20 cycles

    LCD_EN = 1;                                 // enable signal to 1
    LATC = (PORTC & 0x0F) | (adress<<4 & 0xF0); // instruction  for LCD (4 bits)
    for(char i=0;i<20;i++);                     // wait for 20 cycles
    LCD_EN = 0;                                 // enable signal to 0
    __delay_us (50);                            // wait for 50us

    //************** write character ****************

    LCD_RS = 1;                 // data register
    LCD_RW = 0;                 // write to LCD

    LCD_EN = 1;                                 // enable signal to 1
    LATC = (PORTC & 0x0F) | (data & 0xF0);      // instruction LCD (upper 4 bits
    for(char i=0;i<20;i++);                     // wait for 20 cycles
    LCD_EN = 0;                                 // enable signal to 0

    for(char i=0;i<20;i++);                     // wait for 20 cycles
    
    LCD_EN = 1;                                 // enable signal to 1
    LATC = (PORTC & 0x0F) | (data<<4 & 0xF0);   // instruction  for LCD (4 bits)
    for(char i=0;i<20;i++);                     // wait for 20 cycles
    LCD_EN = 0;                                 // enable signal to 0

}

void LCDSendCommand(char command)
{
    TRISCbits.TRISC4=0;         // LCD data lines in output
    TRISCbits.TRISC5=0;
    TRISCbits.TRISC6=0;
    TRISCbits.TRISC7=0;

    LCD_RS = 0;                                 // instruction register
    LCD_RW = 0;                                 // write to LCD

    LCD_EN = 1;                                 // enable signal to 1
    LATC = (PORTC & 0x0F) | (command & 0xF0);    // instruction LCD (upper 4 bits
    for(char i=0;i<20;i++);                     // wait for 20 cycles
    LCD_EN = 0;                                 // enable signal to 0

    for(char i=0;i<20;i++);                     // wait for 20 cycles

    LCD_EN = 1;                                 // enable signal to 1
    LATC = (PORTC & 0x0F) | (command<<4 & 0xF0); // instruction  for LCD (4 bits)
    for(char i=0;i<20;i++);                     // wait for 20 cycles
    LCD_EN = 0;                                 // enable signal to 0
    __delay_us (50);                            // wait for 50us

}

char LCDGetStatus(void)
{
    char result;

    TRISCbits.TRISC4=1;         // LCD data lines in input
    TRISCbits.TRISC5=1;
    TRISCbits.TRISC6=1;
    TRISCbits.TRISC7=1;

    LCD_RS = 0;                 // command register
    LCD_RW = 1;                 // read LCD

    LCD_EN = 1;                                 // enable signal to 1
    for(char i=0;i<20;i++);                     // wait for 20 cycles

    if(PORTCbits.RC7)
        result=LCD_BUSY;
    else
        result=LCD_READY;

    LCD_EN = 0;                                 // enable signal to 0

    for(char i=0;i<20;i++);                     // wait for 20 cycles

    LCD_EN = 1;                                 // enable signal to 1
    for(char i=0;i<20;i++);                     // wait for 20 cycles
    LCD_EN = 0;                                 // enable signal to 0

    TRISCbits.TRISC4=0;         // LCD data lines in output
    TRISCbits.TRISC5=0;
    TRISCbits.TRISC6=0;
    TRISCbits.TRISC7=0;
    
    return result;

}

void LCDInitMsg(void)
{
    LCDSendCharacter(0x0,0x7E);
    __delay_us (50);                            // wait for 50us


    LCDSendCharacter(0x6,'N');
    __delay_us (50);                            // wait for 50us
    LCDSendCharacter(0x7,'a');
    __delay_us (50);                            // wait for 50us
    LCDSendCharacter(0x8,'i');
    __delay_us (50);                            // wait for 50us
    LCDSendCharacter(0x9,'o');
    __delay_us (50);                            // wait for 50us

    LCDSendCharacter(0xF,0x7F);
    __delay_us (50);                            // wait for 50us

    LCDSendCharacter(0x40,0x7E);
    __delay_us (50);                            // wait for 50us


    LCDSendCharacter(0x42,'T');
    __delay_us (50);                            // wait for 50us
    LCDSendCharacter(0x43,'e');
    __delay_us (50);                            // wait for 50us
    LCDSendCharacter(0x44,'c');
    __delay_us (50);                            // wait for 50us
    LCDSendCharacter(0x45,'h');
    __delay_us (50);                            // wait for 50us
    LCDSendCharacter(0x46,'n');
    __delay_us (50);                            // wait for 50us
    LCDSendCharacter(0x47,'o');
    __delay_us (50);                            // wait for 50us
    LCDSendCharacter(0x48,'l');
    __delay_us (50);                            // wait for 50us
    LCDSendCharacter(0x49,'o');
    __delay_us (50);                            // wait for 50us
    LCDSendCharacter(0x4A,'g');
    __delay_us (50);                            // wait for 50us
    LCDSendCharacter(0x4B,'i');
    __delay_us (50);                            // wait for 50us
    LCDSendCharacter(0x4C,'e');
    __delay_us (50);                            // wait for 50us
    LCDSendCharacter(0x4D,'s');
    __delay_us (50);                            // wait for 50us

     LCDSendCharacter(0x4F,0x7F);
    __delay_us (50);                            // wait for 50us


}

void LCDBacklight(char value)
{
    if(value<101)
        CCPR5L = value;

}

void LCDContrast(char value)
{
    if(value<101)
        CCPR2L = (100-value);
}