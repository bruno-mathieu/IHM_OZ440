/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>        /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>       /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>   /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include <stdio.h>

#endif

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "define.h"        /* board level definitions*/
#include "ecanpoll.h"      /* CAN library header file*/
#include "Can_HL.h"
#include "LCD.h"
#include "sound.h"
#include "keybled.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

// structure used to count 10ms tick
struct RTC_counter TickCounter;

//  variable for CAN  FIFO buffer and CAN adress
struct CANTxFifo CANTxFifo;
struct CANRxFifo CANRxFifo;
char LocalCanAdress;

// variable used for sound generation
struct SoundMsg SoundMsg;

// flag for sound timer tick
char SoundTimerTicked;

// variable  used for keyboard
struct KeybMsg KeybMsg;

// variable used for led display
struct LedDisp LedDisp;

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void main(void)
{
    char TempVar;
    unsigned int TempVarInt;

    // Autotest result
    char AutotestResult;
    
    //  variable for CAN TX FIFO buffer
    struct CANTxMsg TempCANTxMsg;

    //  variable for CAN RX FIFO buffer
    struct CANRxMsg TempCANRxMsg;

  
    


    //----------------------------------------------------
    //----------  CPU internal configurations: -----------
    //----------------------------------------------------

    /* Configure the oscillator for the CPU */
    ConfigureOscillator();
    __delay_ms(10);             // wait for Oscillator to be stabilized
    
    // configure CPU GPIO for IMU board
    ConfigureGPIO();

    //Get can adress from rotary switch
    LocalCanAdress = GetCanAdress();
    //CAN controller Initialize
    ECANInitialize();
    //Set MASK and Filters for CAN
    ECANFiltersInit();

    // Timers configuration
    ConfigureTimers();

    // PWM outputs configuration
    ConfigureCCP();

    //----------------------------------------------------
    //----------  Global variables initialisation --------
    //----------------------------------------------------

    // tick counter initialisation
    TickCounter.AccelTick_ms=0;         
    TickCounter.GyroTick_ms=1;
    TickCounter.MagnetTick_ms=2;

    // initialize CAN tx FIFO
    CANTxFifoInit();
    CANRxFifoInit();

    // keyboard variables Initialisation
    KeybMsg.KeybCode=KEYB_RELEASED_CODE;
    KeybMsg.KeybLongPressDelay=200;         //2s for long press detection for default
    KeybMsg.KeybFireDelay=20;               //200ms for long press fire for default

    // LED variables configuration (all off)
    LedDisp.LightedLeds=0;
    LedDisp.LedColor=0;

    //----------------------------------------------------
    //------  external peripheral configurations: --------
    //----------------------------------------------------

    __delay_ms(10);              // wait for reset to be released on external peripherals
    LCDInit();
    __delay_ms(5);
    LCDInitMsg();


    //----------------------------------------------------
    //----------    Ready to go in main loop:  -----------
    //------   interrupts activation and system test -----
    //----------------------------------------------------

    ConfigureInterrupts();

    AutotestResult = SystemTest();

    
    //-----------------------------------------------------
    //-------------  infinite main loop ----------
    //----------------------------------------------------

 
    while(1)
    {
       
    
        //--------------------------------------------------------------------------------
        //--------------------------------------------------------------------------------
        //--------------------------------------------------------------------------------
        //-------------  periodic tasks --------------------------------------------------
        //--------------------------------------------------------------------------------
        //--------------------------------------------------------------------------------
        //--------------------------------------------------------------------------------

        
        //--------------------------------------------------------------------------------
        //-------------  Led management ---------------------------------------------
        //--------------------------------------------------------------------------------

        LedDisplay();

        //--------------------------------------------------------------------------------
        //-------------  Keyboard management ---------------------------------------------
        //--------------------------------------------------------------------------------

        KeybMsg.OldKeybCode=KeybMsg.KeybCode;       // store keybcode before reading
        KeybMsg.KeybCode=KeybRead();                // Keyboard reading

        //-------- check if a new key has been pressed or released
        if(KeybMsg.KeybCode!=KeybMsg.OldKeybCode)
            KeybMsg.KeyPressedEvent=TRUE;
        else
            KeybMsg.KeyPressedEvent=FALSE;

        //-------- if a key press is detected, and not a long press, send can message, with the code detected after key event
        if(KeybMsg.KeyPressedEvent && KeybMsg.KeybCode!=KEYB_RELEASED_CODE && KeybMsg.KeybTimerTicked && KeybMsg.LongPressDetected==FALSE)
        {
                    KeybMsg.KeybTimerTicked=FALSE;
                    KeybMsg.LongPressIndex=0;

                    TempCANTxMsg.data_TX[0] = KeybMsg.KeybCode;
                    TempCANTxMsg.dataLen= KEYB_MSG_LEN;
                    TempCANTxMsg.id = (CAN_MESSAGE_IHM_TYPE << 7 | LocalCanAdress <<4 | KEYB_MSG_TYPE );
                    TempCANTxMsg.flags = ECAN_TX_STD_FRAME;
                    PutCANTxFifo(TempCANTxMsg);
        }

        //-------- if a key released is detected, and not a long press, send can message, with the code detected after key event
        if(KeybMsg.KeyPressedEvent && KeybMsg.KeybCode==KEYB_RELEASED_CODE && KeybMsg.KeybTimerTicked) // && KeybMsg.LongPressDetected==FALSE )
        {
                    KeybMsg.KeybTimerTicked=FALSE;
                    KeybMsg.LongPressIndex=0;

                    TempCANTxMsg.data_TX[0] = KeybMsg.KeybCode;
                    TempCANTxMsg.dataLen= KEYB_MSG_LEN;
                    TempCANTxMsg.id = (CAN_MESSAGE_IHM_TYPE << 7 | LocalCanAdress <<4 | KEYB_MSG_TYPE );
                    TempCANTxMsg.flags = ECAN_TX_STD_FRAME;
                    PutCANTxFifo(TempCANTxMsg);
        }

        //-------- if long press delay expired and long press not detected, switch to long press mode, and send first data frame

        if(KeybMsg.KeybCode!=KEYB_RELEASED_CODE && KeybMsg.LongPressIndex>=KeybMsg.KeybLongPressDelay && KeybMsg.LongPressDetected==FALSE)
        {
            KeybMsg.KeybTimerTicked=FALSE;

            KeybMsg.LongPressDetected=TRUE;
            KeybMsg.LongPressIndex=0;

            TempCANTxMsg.data_TX[0] = KeybMsg.KeybCode | LONGPRESS_OFFSET;
            TempCANTxMsg.dataLen= KEYB_MSG_LEN;
            TempCANTxMsg.id = (CAN_MESSAGE_IHM_TYPE << 7 | LocalCanAdress <<4 | KEYB_MSG_TYPE );
            TempCANTxMsg.flags = ECAN_TX_STD_FRAME;
            PutCANTxFifo(TempCANTxMsg);
            }

        //------- if in long press mode with key pressed, and fire delay expired:
        if(KeybMsg.KeybCode!=KEYB_RELEASED_CODE && KeybMsg.FirePressIndex>=KeybMsg.KeybFireDelay && KeybMsg.LongPressDetected==TRUE)
        {
            KeybMsg.KeybTimerTicked=FALSE;
            KeybMsg.FirePressIndex=0;

            TempCANTxMsg.data_TX[0] = KeybMsg.KeybCode | LONGPRESS_OFFSET;
            TempCANTxMsg.dataLen= KEYB_MSG_LEN;
            TempCANTxMsg.id = (CAN_MESSAGE_IHM_TYPE << 7 | LocalCanAdress <<4 | KEYB_MSG_TYPE );
            TempCANTxMsg.flags = ECAN_TX_STD_FRAME;
            PutCANTxFifo(TempCANTxMsg);
        }

        //------- if the keys are released, while in long press mode:

        if(KeybMsg.KeybCode==KEYB_RELEASED_CODE && KeybMsg.LongPressDetected==TRUE)
        {
            KeybMsg.KeybTimerTicked=FALSE;
            KeybMsg.FirePressIndex=0;
            KeybMsg.LongPressIndex=0;
            KeybMsg.LongPressDetected=FALSE;

        }


        //--------------------------------------------------------------------------------
        //-------------  sound management-------------------------------------------------
        //--------------------------------------------------------------------------------

        if(SoundMsg.SoundType == CONTINOUS && SoundMsg.OnTimeIndex>SoundMsg.OnDuration)
            SoundStop();


        if(SoundTimerTicked==TRUE && SoundMsg.SoundType == INTERMITENT && SoundMsg.CurrentPhase==ONPhase && SoundMsg.OnTimeIndex>=SoundMsg.OnDuration)
        {
            ECCP1CONbits.CCP1M = 0b0000;    //disable sound during off phase

            SoundTimerTicked=FALSE;
            SoundMsg.CurrentPhase=OFFPhase;
            SoundMsg.OnTimeIndex=0;
            SoundMsg.OffTimeIndex=0;

            if(SoundMsg.CycleNumber!=0)
                SoundMsg.CycleIndex++;

        }

        if(SoundTimerTicked==TRUE && SoundMsg.SoundType == INTERMITENT && SoundMsg.CurrentPhase==OFFPhase && SoundMsg.OffTimeIndex>=SoundMsg.OffDuration && SoundMsg.CycleIndex<=SoundMsg.CycleNumber)
        {
            ECCP1CONbits.CCP1M = 0b1100;    //activate sound during on phase

            SoundTimerTicked=FALSE;
            SoundMsg.CurrentPhase=ONPhase;
            SoundMsg.OffTimeIndex=0;
            SoundMsg.OnTimeIndex=0;
     
        }

        if(SoundTimerTicked==TRUE && SoundMsg.SoundType == INTERMITENT && SoundMsg.CurrentPhase==OFFPhase && SoundMsg.OffTimeIndex>SoundMsg.OffDuration && SoundMsg.CycleIndex>SoundMsg.CycleNumber)
        {
            SoundTimerTicked=FALSE;
            SoundStop();
            SoundMsg.CurrentPhase = ONPhase;
            SoundMsg.OffTimeIndex= 0;
            SoundMsg.OnTimeIndex = 0;
        }

        //--------------------------------------------------------------------------------
        //-------------         CAN command interpretor for received messages   ----------
        //--------------------------------------------------------------------------------

        //****  LCD message *****
       
        if (!CANRxFifo.FifoEmpty)
        {

            TempCANRxMsg = GetCANRxFifo();

            if (TempCANRxMsg.id == (CAN_MESSAGE_IHM_TYPE << 7 | LocalCanAdress << 4 | LCD_MSG_TYPE))
            {
                if (TempCANRxMsg.data_RX[0] == LCD_CLEAR_MSG_CODE && TempCANRxMsg.dataLen == LCD_CLEAR_MSG_LEN)
                {
                    LCDClearDisplay();                                                          //  LCD clearing routine
                }
                else if (TempCANRxMsg.data_RX[0] == LCD_SEND_CHAR_MSG_CODE && TempCANRxMsg.dataLen == LCD_SEND_CHAR_MSG_LEN)
                {
                    LCDSendCharacter(TempCANRxMsg.data_RX[1], TempCANRxMsg.data_RX[2]);         //  LCD send char routine
                }
                else if (TempCANRxMsg.data_RX[0] == LCD_SEND_COMMAND_MSG_CODE && TempCANRxMsg.dataLen == LCD_SEND_COMMAND_MSG_LEN)
                {
                    LCDSendCommand(TempCANRxMsg.data_RX[1]);                                    //  LCD send command routine
                }
            }

            //****  LED message *****

            else if (TempCANRxMsg.id == (CAN_MESSAGE_IHM_TYPE << 7 | LocalCanAdress << 4 | LED_MSG_TYPE))
            {
                if (TempCANRxMsg.dataLen == LED_MSG_TYPE)
                {
                    LedDisp.LightedLeds=TempCANRxMsg.data_RX[0];
                    LedDisp.LedColor=TempCANRxMsg.data_RX[1];               //  Store Led parameters
                }
            }

            //****  SOUND message *****

            if (TempCANRxMsg.id == (CAN_MESSAGE_IHM_TYPE << 7 | LocalCanAdress << 4 | SOUND_MSG_TYPE))
            {

                if (TempCANRxMsg.data_RX[0] == SOUND_CONT_MSG_CODE && TempCANRxMsg.dataLen == SOUND_CONT_MSG_LEN)
                {
                    SoundContinuous(TempCANRxMsg.data_RX[1],TempCANRxMsg.data_RX[2],TempCANRxMsg.data_RX[3]);    //  continuous sound routine
                }
                
                else if (TempCANRxMsg.data_RX[0] == SOUND_INT_MSG_CODE && TempCANRxMsg.dataLen == SOUND_INT_MSG_LEN)
                {
                    SoundIntermittent(TempCANRxMsg.data_RX[1],TempCANRxMsg.data_RX[2],TempCANRxMsg.data_RX[3],TempCANRxMsg.data_RX[4],TempCANRxMsg.data_RX[5]); //  Interm. sound routine
                }
                else if (TempCANRxMsg.data_RX[0] == SOUND_STOP_MSG_CODE && TempCANRxMsg.dataLen == SOUND_STOP_MSG_LEN)
                {
                    SoundStop();        // stop sound routine
                }
            }

            //****  STATUS message *****

            if (TempCANRxMsg.id == (CAN_MESSAGE_IHM_TYPE << 7 | LocalCanAdress << 4 | STATUS_MSG_TYPE))
            {
                if (TempCANRxMsg.dataLen == STATUS_MSG_LEN && TempCANRxMsg.flags == ECAN_RX_RTR_FRAME)
                {
                    // insert status message response to RTR request

                    LCDGetStatus();
                    TempCANTxMsg.data_TX[0] = LCDGetStatus() << 1 | AutotestResult;
                    TempCANTxMsg.dataLen= STATUS_MSG_LEN;
                    TempCANTxMsg.id = (CAN_MESSAGE_IHM_TYPE << 7 | LocalCanAdress <<4 | STATUS_MSG_TYPE );
                    TempCANTxMsg.flags = ECAN_TX_STD_FRAME;
                    PutCANTxFifo(TempCANTxMsg);


                }
            }

            //****  keyboard configuration message *****

            else if (TempCANRxMsg.id == (CAN_MESSAGE_IHM_TYPE << 7 | LocalCanAdress << 4 | KEYB_CONFIG_MSG_TYPE))
            {
                if (TempCANRxMsg.dataLen == KEYB_CONFIG_MSG_LEN)
                {
                    KeybConfiguration(TempCANRxMsg.data_RX[0], TempCANRxMsg.data_RX[1]);    //  KEYBOARD configuration routine
                }
            }

            //****  Contrast configuration message *****

            else if (TempCANRxMsg.id == (CAN_MESSAGE_IHM_TYPE << 7 | LocalCanAdress << 4 | LCD_CONTRAST_MSG_TYPE))
            {
                if (TempCANRxMsg.dataLen == LCD_CONTRAST_MSG_LEN)
                {
                    LCDContrast(TempCANRxMsg.data_RX[0]);                                       // LCD contrast configuration routine
                }
            }

            //****  Backlight configuration message *****

            else if (TempCANRxMsg.id == (CAN_MESSAGE_IHM_TYPE << 7 | LocalCanAdress << 4 | LCD_BACKLIGHT_MSG_TYPE))
            {
                if (TempCANRxMsg.dataLen == LCD_BACKLIGHT_MSG_LEN)
                {
                    LCDBacklight(TempCANRxMsg.data_RX[0]);                                      //  LCD backlight configuration routine
                }
            }

            //****  Software Version message *****

            else if (TempCANRxMsg.id == (CAN_MESSAGE_IHM_TYPE << 7 | LocalCanAdress << 4 | SOFT_VERSION_MESSAGE_ADRESS))
            {
                if (TempCANRxMsg.flags == ECAN_RX_RTR_FRAME)
                {
                    TempCANTxMsg.data_TX[0]=MAJOR_SW_VERSION;
                    TempCANTxMsg.data_TX[1]=MINOR_SW_VERSION;
                    TempCANTxMsg.dataLen= SOFT_VERSION_MESSAGE_LEN;
                    TempCANTxMsg.id = (CAN_MESSAGE_IHM_TYPE << 7 | LocalCanAdress <<4 | SOFT_VERSION_MESSAGE_ADRESS );
                    TempCANTxMsg.flags = ECAN_TX_STD_FRAME;
                    PutCANTxFifo(TempCANTxMsg);
                }
            }

            //****  Hardware Version message *****

            else if (TempCANRxMsg.id == (CAN_MESSAGE_IHM_TYPE << 7 | LocalCanAdress << 4 | BOARD_VERSION_MESSAGE_ADRESS))
            {
                
                if (TempCANRxMsg.flags == ECAN_RX_RTR_FRAME)
                {
                    TempCANTxMsg.data_TX[0]=BOARD_NUMBER;
                    TempCANTxMsg.data_TX[1]=BOARD_REVISION;
                    TempCANTxMsg.dataLen= BOARD_VERSION_MESSAGE_LEN;
                    TempCANTxMsg.id = (CAN_MESSAGE_IHM_TYPE << 7 | LocalCanAdress <<4 | BOARD_VERSION_MESSAGE_ADRESS );
                    TempCANTxMsg.flags = ECAN_TX_STD_FRAME;
                    PutCANTxFifo(TempCANTxMsg);
                }
            }
        }

        //--------------------------------------------------------------------------------
        // ---  Send can message if TXB0 buffer free, and data available in CAN TX FIFO --
        //--------------------------------------------------------------------------------
        
        if(!CANTxFifo.FifoEmpty && !TXB0CONbits.TXREQ)          // if fifo is not empty and buffer0 empty
        {
            TempCANTxMsg=GetCANTxFifo();
            ECANSendMessage(TempCANTxMsg.id,TempCANTxMsg.data_TX,TempCANTxMsg.dataLen,TempCANTxMsg.flags);  // fill tx buffer with Fifo data
        }


    }

}

