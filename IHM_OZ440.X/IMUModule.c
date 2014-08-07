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
#include "IMUModule.h"
#include "define.h"

/*********************************************************************
 * Function:        void IMUInitRegisters(void)
 *
 * Overview:        Use this function to initialize IMU registers according
 *                  to IMU 1v0 board, and BMX055 chip
 *
 * PreCondition:    GPIO and SPI configured, POR terminated
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 ********************************************************************/

void IMUInitRegisters(void)
{

    IMUWrite(0x10,0x00,MAGNET_ID);          // set lowpass filter to 7HZ for accelerometer
    __delay_ms(1);

    IMUWrite(0x0F,0x03,GYRO_ID);            // set the Gyro range to 250deg/sec
    __delay_ms(1);
    IMUWrite(0x10,0x07,GYRO_ID);            // reduce gyro bandwith to 20Hz
    __delay_ms(1);

    IMUWrite(0x4B,0x01,MAGNET_ID);          // set magnet sensor powered on
    __delay_ms(1);
    IMUWrite(0x4E,0x05,MAGNET_ID);          // activate axis, disable interrupts
    __delay_ms(1);
    IMUWrite(0x51,23,MAGNET_ID);            // set XY repetitions to 46
    __delay_ms(1);
    IMUWrite(0x51,82,MAGNET_ID);            // set Z repetitions to 83
    __delay_ms(1);
    IMUWrite(0x4C,0x28,MAGNET_ID);          // set magnet sensor in normal mode, 20Hz update rate
    __delay_ms(1);

}

/*********************************************************************
 * Function:        unsigned char IMUAutotest(void)
 *
 * Overview:        Use this function to check communication with BMX055 chip
 *
 * PreCondition:    GPIO and SPI configured, POR terminated, IMUinitRegister loaded
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 ********************************************************************/

unsigned char IMUAutotest(void)
{
    unsigned char Result, TempVar;

    Result=0;

    TempVar = IMURead(0x00,ACCEL_ID);
    if(TempVar==ACCEL_MANUFACTURING_CODE)
        Result = Result | 0x01;

    TempVar = IMURead(0x00,GYRO_ID);
    if(TempVar==GYRO_MANUFACTURING_CODE)
        Result = Result | 0x02;

    TempVar = IMURead(0x40,MAGNET_ID);
    if(TempVar==MAGNET_MANUFACTURING_CODE)
        Result = Result | 0x04;

    return Result;

}
/*********************************************************************
 * Function:        struct IMUData IMUUpdateData(void)
 *
 * Overview:        Update IMU data with new values
 *
 * PreCondition:    IMU chip already configured
 *
 * Input:           None
 *
 * Output:          IMUData type structure
 *
 * Side Effects:    interrupts disabled during SPI transmission
 ********************************************************************/

struct IMUData IMUUpdateData(void)
{
    struct IMUData temp_data;
    unsigned int Temp_Var1=0;
    unsigned int Temp_Var2=0;

    //------ accel sensor Acquisition -----

    Temp_Var1= (IMURead(0x02,ACCEL_ID) & 0xF0)>>4;      // get and store X accel LSB data with 4 bit shift
    Temp_Var2 = IMURead(0x03,ACCEL_ID);                 // get X accel MSB data
    temp_data.XAccelerationData = (Temp_Var2<<4)|Temp_Var1;

    Temp_Var1=0;
    Temp_Var2=0;
    Temp_Var1= (IMURead(0x04,ACCEL_ID) & 0xF0)>>4;      // get and store Y accel LSB data with 4 bit shift
    Temp_Var2 = IMURead(0x05,ACCEL_ID);                 // get Y accel MSB data
    temp_data.YAccelerationData = (Temp_Var2<<4)|Temp_Var1;

    Temp_Var1=0;
    Temp_Var2=0;
    Temp_Var1= (IMURead(0x06,ACCEL_ID) & 0xF0)>>4;      // get and store Z accel LSB data with 4 bit shift
    Temp_Var2 = IMURead(0x07,ACCEL_ID);                 // get Z accel MSB data
    temp_data.ZAccelerationData = (Temp_Var2<<4)|Temp_Var1;

    //------ Gyro sensor Acquisition -----

    Temp_Var1=0;
    Temp_Var2=0;
    Temp_Var1= IMURead(0x02,GYRO_ID);                 // get and store X gyro LSB data
    Temp_Var2 = IMURead(0x03,GYRO_ID);                 // get X gyro MSB data
    temp_data.XGyroscopeData = (Temp_Var2<<8)|Temp_Var1;

    Temp_Var1=0;
    Temp_Var2=0;
    Temp_Var1= IMURead(0x04,GYRO_ID);                 // get and store Y gyro LSB data
    Temp_Var2 = IMURead(0x05,GYRO_ID);                 // get Y gyro MSB data
    temp_data.YGyroscopeData = (Temp_Var2<<8)|Temp_Var1;

    Temp_Var1=0;
    Temp_Var2=0;
    Temp_Var1= IMURead(0x06,GYRO_ID);                 // get and store Z gyro LSB data
    Temp_Var2 = IMURead(0x07,GYRO_ID);                 // get Z gyro MSB data
    temp_data.ZGyroscopeData = (Temp_Var2<<8)|Temp_Var1;

    //------ magnet sensor Acquisition -----

    Temp_Var1=0;
    Temp_Var2=0;
    Temp_Var1= (IMURead(0x42,MAGNET_ID) & 0xF8)>>3;      // get and store X magnet LSB data with 3 bit shift
    Temp_Var2 = IMURead(0x43,MAGNET_ID);                 // get X magnet MSB data
    temp_data.XMagnetData = (Temp_Var2<<5)|Temp_Var1;

    Temp_Var1=0;
    Temp_Var2=0;
    Temp_Var1= (IMURead(0x44,MAGNET_ID) & 0xF8)>>3;      // get and store Y magnet LSB data with 3 bit shift
    Temp_Var2 = IMURead(0x45,MAGNET_ID);                 // get Y magnet MSB data
    temp_data.YMagnetData = (Temp_Var2<<5)|Temp_Var1;

    Temp_Var1=0;
    Temp_Var2=0;
    Temp_Var1= (IMURead(0x46,MAGNET_ID) & 0xF8)>>3;      // get and store Y magnet LSB data with 3 bit shift
    Temp_Var2 = IMURead(0x47,MAGNET_ID);                 // get Y magnet MSB data
    temp_data.ZMagnetData = (Temp_Var2<<5)|Temp_Var1;

    Temp_Var1=0;
    Temp_Var2=0;
    Temp_Var1= (IMURead(0x48,MAGNET_ID) & 0xFC)>>2;      // get and store Y magnet LSB data with 3 bit shift
    Temp_Var2 = IMURead(0x49,MAGNET_ID);                 // get Y magnet MSB data
    temp_data.MagnetResistance = (Temp_Var2<<6)|Temp_Var1;

    //------ temperature sensor Acquisition -----

    temp_data.Temperature = IMURead(0x08,ACCEL_ID);      // reads temperature from accelerometer sensor
    return temp_data;

}


/*********************************************************************
 * Function:        char IMURead(char adress,char Device)
 *
 * Overview:        get IMU data, at specified adress, on the specified device
 *                  see define.h for device selection
 *
 * PreCondition:    IMU chip already configured
 *
 * Input:           adress (char), Device
 *
 * Output:          data read (char)
 *
 * Side Effects:    interrupts disabled during SPI transmission
 ********************************************************************/

char IMURead(char adress,char Device)
{
    char TempData=0;
    char Result=0;

    
    TempData = (adress | 0b10000000);          //set RW bit, and adress (clipped to 7 bits)
  
    if(Device==ACCEL_ID)                        // select the right CS to assert according to device selection
        ACCEL_SPI_CS = 0;
    else if(Device==GYRO_ID)
        GYRO_SPI_CS = 0;
    else if(Device==MAGNET_ID)
        MAGNET_SPI_CS=0;

    SPIWrite(TempData);
    Result = SPIRead(0);

    ACCEL_SPI_CS = 1;
    GYRO_SPI_CS = 1;
    MAGNET_SPI_CS= 1;

    return Result;

}

/*********************************************************************
 * Function:        char IMUWrite(char adress,char Device)
 *
 * Overview:        put IMU data, at specified adress, on the specified device
 *                  see define.h for device selection
 *
 * PreCondition:    IMU chip already configured
 *
 * Input:           adress (char), data (char), Device
 *
 * Output:          None
 *
 * Side Effects:    interrupts disabled during SPI transmission
 ********************************************************************/

void IMUWrite(char adress,char data,char Device)
{
    char TempData=0;
    char Result=0;

    TempData = (adress & 0b01111111);          //set RW bit, and adress (clipped)
    
    if(Device==ACCEL_ID)                       // select the right CS to assert according to device selection
        ACCEL_SPI_CS = 0;
    else if(Device==GYRO_ID)
        GYRO_SPI_CS = 0;
    else if(Device==MAGNET_ID)
        MAGNET_SPI_CS=0;


    SPIWrite(TempData);
    SPIWrite(data);

    ACCEL_SPI_CS = 1;
    GYRO_SPI_CS = 1;
    MAGNET_SPI_CS=1;


}
