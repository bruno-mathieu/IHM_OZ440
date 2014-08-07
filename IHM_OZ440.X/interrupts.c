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
#include "Can_HL.h"
#include "sound.h"
#include "frequency_LUT.h"
#endif


extern struct CANRxFifo CANRxFifo;

extern struct SoundMsg SoundMsg;
extern char SoundTimerTicked;
extern struct KeybMsg KeybMsg;

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

/* High-priority service */

#if defined(__XC) || defined(HI_TECH_C)
void interrupt high_isr(void)
#elif defined (__18CXX)
#pragma code high_isr=0x08
#pragma interrupt high_isr
void high_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif

{
    struct CANRxMsg CANRxNewMessage;

      /* This code stub shows general interrupt handling.  Note that these
      conditional statements are not handled within 3 seperate if blocks.
      Do not use a seperate if block for each interrupt flag to avoid run
      time errors. */

    if(PIR5bits.RXB0IF==1 | PIR5bits.RXB1IF==1)          // new CAN message received in buffer0 or buffer1
        {

            ECANReceiveMessage(&CANRxNewMessage.id, CANRxNewMessage.data_RX, &CANRxNewMessage.dataLen, &CANRxNewMessage.flags);
            PutCANRxFifo(CANRxNewMessage);

            PIR5bits.RXB0IF=0;      // clear interrupt flags
            PIR5bits.RXB1IF=0;
        }

   


}


/* Low-priority interrupt routine */
#if defined(__XC) || defined(HI_TECH_C)
void low_priority interrupt low_isr(void)
#elif defined (__18CXX)
#pragma code low_isr=0x18
#pragma interruptlow low_isr
void low_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif
{

     
    if(INTCONbits.TMR0IF)           // TIMER0 overflow interrupt
    {
        INTCONbits.TMR0IF=0;        // Clear interrupt flag

        SoundTimerTicked=TRUE;
        KeybMsg.KeybTimerTicked=TRUE;
        
        //--------------- sound message counter management --------------

        if(SoundMsg.SoundType==CONTINOUS)
        {
            SoundMsg.OnTimeIndex++;
        }

        if(SoundMsg.SoundType==INTERMITENT && SoundMsg.CurrentPhase==ONPhase)
        {
            SoundMsg.OnTimeIndex++;
        }
        
        if(SoundMsg.SoundType==INTERMITENT && SoundMsg.CurrentPhase==OFFPhase)
        {
            SoundMsg.OffTimeIndex++;
        }

        //--------------- Keyboard timer management --------------

        if( KeybMsg.KeybCode!=KEYB_RELEASED_CODE && !KeybMsg.LongPressDetected)
        {
            KeybMsg.LongPressIndex++;
        }

         if( KeybMsg.KeybCode!=KEYB_RELEASED_CODE && KeybMsg.LongPressDetected)
        {
            KeybMsg.FirePressIndex++;
        }

        TMR0H = TMR0H_INIT;
        TMR0L = TMR0L_INIT;         // preload for 10ms overflow

    }

        
}
