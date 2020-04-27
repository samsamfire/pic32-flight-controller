
#include "i2c.h"

#define AT24_address 0xAE



void AT24_writeByte(int reg_high, int reg_low, int value){
     I2C4_start();
     I2C4_write(AT24_address);
     I2C4_write(reg_high);
     I2C4_write(reg_low);
     I2C4_write(value);
     I2C4_stop();
     }
void AT24_writeByteMultiple(int reg_high, int reg_low, char *eeprom_buff,int len){
     int i=0;
     I2C4_start();
     I2C4_write(AT24_address);
     I2C4_write(reg_high);
     I2C4_write(reg_low);

     while(i<len){
                  I2C4_write(eeprom_buff[i]);
                  i+=1;
                  }
     I2C4_stop();
     }

     
char AT24_readByte(int reg_high, int reg_low){
    char reslt=0;
    I2C4_Start();              // issue I2C start signal
    I2C4_Write(AT24_address);          // send byte via I2C  (device address + W)
    I2C4_Write(reg_high);         // send byte (data address)
    I2C4_Write(reg_low);         // send byte (data address)
    I2C4_Restart();            // issue I2C signal repeated start
    I2C4_Write(AT24_address+1);          // send byte (device address + R)
    reslt = I2C4_Read(1);      // Read the data (NO acknowledge)
    I2C4_Stop();
    return reslt;

    }
    
char AT24_readByteMultiple(int reg_high, int reg_low, char *eeprom_buff, int len){
    int i=0;
    I2C4_Start();              // issue I2C start signal
    I2C4_Write(AT24_address);          // send byte via I2C  (device address + W)
    I2C4_Write(reg_high);         // send byte (data address)
    I2C4_Write(reg_low);         // send byte (data address)
    I2C4_Restart();            // issue I2C signal repeated start
    I2C4_Write(AT24_address+1);          // send byte (device address + R)
    while(i<len-1){
                 eeprom_buff[i]=I2C4_Read(0);
                 i+=1;
                 }
    eeprom_buff[i]=I2C4_Read(1);

    I2C4_Stop();
    return 1;

    }