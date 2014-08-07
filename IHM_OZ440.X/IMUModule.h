/* 
 * File:   IMUModule.h
 * Author: Bruno
 *
 * Created on 18 juin 2014, 17:18
 */

#ifndef IMUMODULE_H
#define	IMUMODULE_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* IMUMODULE_H */

char IMURead(char adress,char Device);
void IMUWrite(char adress,char data,char Device);

void IMUInitRegisters(void);
struct IMUData IMUUpdateData(void);

unsigned char IMUAutotest(void);