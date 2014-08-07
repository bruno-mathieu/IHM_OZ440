/* 
 * File:   Can.h
 * Author: Bruno
 *
 * Created on 13 juin 2014, 17:29
 */

#ifndef CAN_H
#define	CAN_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* CAN_H */

struct CanMessage
{
    char identifier;
    char DLC;
    bool RTR;
    char data[7];
};

