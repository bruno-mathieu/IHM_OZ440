/* 
 * File:   LCD.h
 * Author: Bruno
 *
 * Created on 28 juillet 2014, 16:00
 */

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */


void LCDClearDisplay(void);
void LCDSendCharacter(char adress,char data);
void LCDSendCommand(char command);
void LCDBacklight(char value);
void LCDContrast(char value);
void LCDInit(void);
char LCDGetStatus(void);
void LCDInitMsg(void);