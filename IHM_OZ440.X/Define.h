/* 
 * File:   Define.h
 * Author: Bruno
 *
 * Created on 13 juin 2014, 16:13
 */

#ifndef DEFINE_H
#define	DEFINE_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* DEFINE_H */

#define MAJOR_SW_VERSION 1
#define MINOR_SW_VERSION 1

#define BOARD_NUMBER 1
#define BOARD_REVISION 1

#define _XTAL_FREQ 64000000

#define TMR0H_INIT  0xFB              // TMR0 value for 10ms tick
#define TMR0L_INIT  0x1E              // TMR0 value for 10ms tick

#define IHM_TICK_PERIOD 4             // IHM polling ticks each 10ms




#define CAN_MESSAGE_IHM_TYPE 0x07


// ***  messages dedicated to LCD display  ************

#define LCD_MSG_TYPE 0

#define LCD_CLEAR_MSG_CODE 0x1
#define LCD_SEND_CHAR_MSG_CODE 0x2
#define LCD_SEND_COMMAND_MSG_CODE 0x3

#define LCD_CLEAR_MSG_LEN 1
#define LCD_SEND_CHAR_MSG_LEN 3
#define LCD_SEND_COMMAND_MSG_LEN 2

// ***  messages dedicated to Keyboard  ************

#define KEYB_MSG_TYPE 0x1
#define KEYB_MSG_LEN 1

// ***  messages dedicated to IHM Leds  ************

#define LED_MSG_TYPE        0x2
#define LED_MSG_LEN         2

// ***  messages dedicated to Buzzer  ************

#define SOUND_MSG_TYPE      0x3

#define SOUND_CONT_MSG_CODE 0x1
#define SOUND_INT_MSG_CODE  0x2
#define SOUND_STOP_MSG_CODE 0x3

#define SOUND_CONT_MSG_LEN  0x4
#define SOUND_INT_MSG_LEN   0x6
#define SOUND_STOP_MSG_LEN  0x1

// **** testability message ************

#define STATUS_MSG_TYPE     0x4
#define STATUS_MSG_LEN      0x1

// **** Keyboard configuration message ************

#define KEYB_CONFIG_MSG_TYPE     0x5
#define KEYB_CONFIG_MSG_LEN      0x2

// **** LCD contrast message ************

#define LCD_CONTRAST_MSG_TYPE     0x7
#define LCD_CONTRAST_MSG_LEN      0x1

// **** LCD Backlight message ************

#define LCD_BACKLIGHT_MSG_TYPE     0x8
#define LCD_BACKLIGHT_MSG_LEN      0x1




#define SOFT_VERSION_MESSAGE_ADRESS 0x6
#define SOFT_VERSION_MESSAGE_LEN 2

#define BOARD_VERSION_MESSAGE_ADRESS 0xF
#define BOARD_VERSION_MESSAGE_LEN 2

// sound types

#define CONTINOUS 1
#define INTERMITENT 2
#define OFF 0

// sound phases
#define ONPhase 1
#define OFFPhase 2

// keyboard define

#define KEYB_RELEASED_CODE 0x00

#define CANTX_FIFO_SIZE 10
#define CANRX_FIFO_SIZE 10


#define LCD_RS LATEbits.LATE0
#define LCD_RW LATEbits.LATE1
#define LCD_EN LATEbits.LATE2


#define TRUE    1
#define FALSE   0

#define LCD_BUSY 1
#define LCD_READY 0

