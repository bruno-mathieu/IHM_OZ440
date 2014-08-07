/* 
 * File:   ISMModule.h
 * Author: Bruno
 *
 * Created on 18 juin 2014, 14:06
 */

#ifndef ISMMODULE_H
#define	ISMMODULE_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* ISMMODULE_H */


char ISMReadConfig(char adress);
void ISMWriteConfig(char adress,char data);

char ISMReadData(void);

void ISMWriteData(char data);
