
#include "ECANPoll.h"
#include "Define.h"

/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/

/* TODO Define system operating frequency */

/* Microcontroller MIPs (FCY) */
#define SYS_FREQ        8000000L
#define FCY             SYS_FREQ/4


/******************************************************************************/
/* Variable types declaration                                                      */
/******************************************************************************/



struct RTC_counter {
    unsigned char AccelTick_ms;
    unsigned char GyroTick_ms;
    unsigned char MagnetTick_ms;

};

struct CANTxMsg{

    unsigned long id;
    BYTE data_TX[8];
    BYTE dataLen;
    ECAN_TX_MSG_FLAGS flags;

};

struct CANRxMsg{

    unsigned long id;
    BYTE data_RX[8];
    BYTE dataLen;
    ECAN_RX_MSG_FLAGS flags;

};

struct CANTxFifo{

    struct CANTxMsg CANMsg[CANTX_FIFO_SIZE];

    unsigned char LowIndex;
    unsigned char HighIndex;
    unsigned char Fifofull;
    unsigned char FifoEmpty;

};

struct CANRxFifo{

    struct CANRxMsg CANMsg[CANRX_FIFO_SIZE];

    unsigned char LowIndex;
    unsigned char HighIndex;
    unsigned char Fifofull;
    unsigned char FifoEmpty;

};


struct SoundMsg{

    char SoundType;
    char Volume;
    char OnDuration;
    char OffDuration;
    char CurrentPhase;
    char CycleNumber;
    char CycleIndex;
    unsigned int Frequency;
    char OnTimeIndex;
    char OffTimeIndex;

};

struct KeybMsg{

    char KeybCode;
    char OldKeybCode;
    char KeyPressedEvent;
    char LongPressDetected;
    char FirePressIndex;
    char LongPressIndex;
    char KeybTimerTicked;
    char KeybLongPressDelay;
    char KeybFireDelay;
    
};

struct LedDisp{
    char LightedLeds;
    char LedColor;
    char LedDiplayCycle;

};

/******************************************************************************/
/* System Function Prototypes                                                 */
/******************************************************************************/


/* Custom oscillator configuration funtions, reset source evaluation
functions, and other non-peripheral microcontroller initialization functions
go here. */

void ConfigureOscillator(void);     /* Handles clock switching/osc initialization */
void ConfigureGPIO(void);           /* Handles GPIO configuration*/
void ConfigureAnalog(void);         /* Handles ADC configuration for CAN adress read on startup*/
void ConfigureInterrupts(void);     /* Handles Interrupts configuration*/

void ConfigureTimers(void);         /* handles timers configuration*/
char SystemTest(void);
void ConfigureCCP(void);
char GetCanAdress(void);