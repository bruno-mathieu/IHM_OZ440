
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
#include "Can_HL.h"
#include "define.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/* variable declard in Main                                                   */
/******************************************************************************/

extern struct CANTxFifo CANTxFifo;
extern struct CANRxFifo CANRxFifo;
extern char LocalCanAdress;

/******************************************************************************/

void CANTxFifoInit(void)
{
   CANTxFifo.FifoEmpty=1;
   CANTxFifo.Fifofull=0;
   CANTxFifo.LowIndex=0;
   CANTxFifo.HighIndex=0;
}

void CANRxFifoInit(void)
{
   CANRxFifo.FifoEmpty=1;
   CANRxFifo.Fifofull=0;
   CANRxFifo.LowIndex=0;
   CANRxFifo.HighIndex=0;
}

void PutCANTxFifo(struct CANTxMsg PutMessage)
{
    
    if(!CANTxFifo.Fifofull)             // if the fifo is not full...
    {
        INTCONbits.GIE=0;                   // disable interrupts during Fifo filling
        CANTxFifo.CANMsg[CANTxFifo.HighIndex]=PutMessage;   // write the message in parameters
        CANTxFifo.FifoEmpty=0;                              // so the fifo is not empty
        CANTxFifo.HighIndex++;                              // incr High index

        if(CANTxFifo.HighIndex==(CANTX_FIFO_SIZE))          // if the high index match to fifo size, go back to 0
            CANTxFifo.HighIndex=0;
        
        if(CANTxFifo.HighIndex==CANTxFifo.LowIndex)     //after last message been written, is the buffer full?
            CANTxFifo.Fifofull=1;

        INTCONbits.GIE=1;                   // enable interrupts after Fifo filling
    }

}


struct CANTxMsg GetCANTxFifo(void)
{
    struct CANTxMsg LocalCANTXMsg;

    
    if(!CANTxFifo.FifoEmpty)            // if the fifo is not empty....
    {
        INTCONbits.GIE=0;                   // disable interrupts during Fifo reading

        LocalCANTXMsg=CANTxFifo.CANMsg[CANTxFifo.LowIndex];
        CANTxFifo.Fifofull=0;                              // so the fifo is not full
        CANTxFifo.LowIndex++;

        if(CANTxFifo.LowIndex==(CANTX_FIFO_SIZE))          // if the low index match to fifo size, go back to 0
            CANTxFifo.LowIndex=0;

        if(CANTxFifo.HighIndex==CANTxFifo.LowIndex)     //after last message been read, is the buffer empty?
            CANTxFifo.FifoEmpty=1;

        INTCONbits.GIE=1;                   // enable interrupts after Fifo reading
        return LocalCANTXMsg;
    }
    else
    {
        LocalCANTXMsg.id=0;
        return LocalCANTXMsg;       // fifo empty, return ID=0
    }

}

void PutCANRxFifo(struct CANRxMsg PutMessage)
{

    if(!CANRxFifo.Fifofull)             // if the fifo is not full...
    {
        CANRxFifo.CANMsg[CANRxFifo.HighIndex]=PutMessage;   // write the message in parameters
        CANRxFifo.FifoEmpty=0;                              // so the fifo is not empty
        CANRxFifo.HighIndex++;                              // incr High index

        if(CANRxFifo.HighIndex==(CANTX_FIFO_SIZE))          // if the high index match to fifo size, go back to 0
            CANRxFifo.HighIndex=0;

        if(CANRxFifo.HighIndex==CANRxFifo.LowIndex)     //after last message been written, is the buffer full?
            CANRxFifo.Fifofull=1;
    }

}

struct CANRxMsg GetCANRxFifo(void)
{
    struct CANRxMsg LocalCANRXMsg;


    if(!CANRxFifo.FifoEmpty)            // if the fifo is not empty....
    {
        INTCONbits.GIE=0;                   // disable interrupts during Fifo reading

        LocalCANRXMsg=CANRxFifo.CANMsg[CANRxFifo.LowIndex];
        CANRxFifo.Fifofull=0;                              // so the fifo is not full
        CANRxFifo.LowIndex++;

        if(CANRxFifo.LowIndex==(CANTX_FIFO_SIZE))          // if the low index match to fifo size, go back to 0
            CANRxFifo.LowIndex=0;

        if(CANRxFifo.HighIndex==CANRxFifo.LowIndex)     //after last message been read, is the buffer empty?
            CANRxFifo.FifoEmpty=1;

        INTCONbits.GIE=1;                   // enable interrupts after Fifo reading
        return LocalCANRXMsg;
    }
    else
    {
        LocalCANRXMsg.id=0;
        return LocalCANRXMsg;       // fifo empty, return ID=0
    }

}

 void ECANFiltersInit(void)
 {
     unsigned int mask;

     // mask for message TYPE and device adress (see ICD)
     RXM0SIDH = 0b11111110;
     RXM0SIDL = 0b00000000;

     RXM1SIDH = 0b11111110;
     RXM1SIDL = 0b00000000;

     // acceptance filter for IMU type messages

     mask = (CAN_MESSAGE_IHM_TYPE<<7 | LocalCanAdress<<4 |0000);
     RXF0SIDH = (char) (mask >> 3);
     RXF0SIDL = (char) (mask << 5);

   
     // disable other filters

     RXF4SIDH = 0;
     RXF4SIDL = 0;
     RXF5SIDH = 0;
     RXF5SIDL = 0;

 }