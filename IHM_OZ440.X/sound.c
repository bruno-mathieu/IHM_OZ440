
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
#include "sound.h"
#include "define.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/* variable declared in Main                                                   */
/******************************************************************************/

extern struct SoundMsg SoundMsg;

/******************************************************************************/

void SoundContinuous(char freq, char duration, char volume)
{

    int temp_volume;

    SoundMsg.SoundType = CONTINOUS;

    SoundMsg.OnDuration = duration;
    SoundMsg.Volume = volume;
    SoundMsg.Frequency = freq;
    SoundMsg.CurrentPhase = ONPhase;

    SoundMsg.OnTimeIndex = 0;
    SoundMsg.OffTimeIndex= 0;
    SoundMsg.CycleNumber = 0;
    SoundMsg.OffDuration = 0;

    PR4=0xFF;

    temp_volume = PR4 * volume;
    temp_volume = temp_volume/200;
    
    CCPR1L=(char)temp_volume;

    ECCP1CONbits.CCP1M = 0b1100;    //all outputs active high


}

void SoundIntermittent(char freq, char soundduration, char SilentDuration, char cycles, char volume)
{
    int temp_volume;
    
    SoundMsg.SoundType = INTERMITENT;

    SoundMsg.OnDuration = soundduration;
    SoundMsg.OffDuration = SilentDuration;

    SoundMsg.Volume = volume;
    SoundMsg.Frequency = freq;
    SoundMsg.CurrentPhase = ONPhase;

    SoundMsg.OnTimeIndex = 0;
    SoundMsg.OffTimeIndex= 0;
    SoundMsg.CycleNumber = cycles;
    SoundMsg.CycleIndex =0;

    PR4=0xFF;

    temp_volume = PR4 * volume;
    temp_volume = temp_volume/200;

    CCPR1L=(char)temp_volume;

    ECCP1CONbits.CCP1M = 0b1100;    //all outputs active high

}

void SoundStop(void)
{
    SoundMsg.SoundType = OFF;

    ECCP1CONbits.CCP1M = 0b0000;    // disable ECCP
    LATDbits.LATD4 = 0;             // set output to 0
    LATDbits.LATD5 = 0;             // set output to 0

}