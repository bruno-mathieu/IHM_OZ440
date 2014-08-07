/* 
 * File:   Can_HL.h
 * Author: Bruno
 *
 * Created on 23 juin 2014, 15:23
 */

#ifndef CAN_HL_H
#define	CAN_HL_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* CAN_HL_H */

void CANTxFifoInit(void);
void CANRxFifoInit(void);

void PutCANTxFifo(struct CANTxMsg PutMessage);
struct CANTxMsg GetCANTxFifo(void);
void PutCANRxFifo(struct CANRxMsg PutMessage);
struct CANRxMsg GetCANRxFifo(void);

void ECANFiltersInit(void);