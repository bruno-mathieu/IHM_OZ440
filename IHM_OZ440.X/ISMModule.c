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
#include "ISMModule.h"
#include "define.h"


char ISMReadConfig(char adress)
{
    char TempData=0;
    char Result=0;

    TempData = (adress & 0x1F);         //mask for stop bit and RW bit
    TempData = TempData << 1;
    TempData = (TempData | 0b01000000) & 0xFE;      //set RW, sotp, and start bit

    ISM_CON_CS = 0;
    SPIWrite(TempData);
    Result = SPIRead(0);
    ISM_CON_CS = 1;

    return Result;
    
}

void ISMWriteConfig(char adress,char data)
{
    char TempData=0;
    char Result=0;

    TempData = (adress & 0x1F);         //mask for stop bit and RW bit
    TempData = TempData << 1;
    TempData = (TempData ) & 0x3E;      //set RW, sotp, and start bit

    ISM_CON_CS = 0;
    SPIWrite(TempData);
    SPIWrite(data);
    ISM_CON_CS = 1;

}

char ISMReadData(void)
{
    char Result;

    ISM_DAT_CS = 0;
    Result = SPIRead(0);
    ISM_DAT_CS = 1;

    return Result;
}

void ISMWriteData(char data)
{
    
    ISM_DAT_CS = 0;
    SPIWrite(data);
    ISM_DAT_CS = 1;

}