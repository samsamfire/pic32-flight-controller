#include "i2c.h"

void writeByte(int address, int reg, int value) {
     I2C5_start();
     I2C5_write(address);
     I2C5_write(reg);
     I2C5_write(value);
     I2C5_stop();
     }
     
char readByte(int address, int reg) {
    char result=0;
    I2C5_start();
    I2C5_write(address);
    I2C5_write(reg);
    I2C5_restart();
    I2C5_write(address + 1);
    result = I2C5_read(1);
    I2C5_stop();
    return result;
    }

void i2c_write_multiple(int adress, int reg, char buff[], int length){
     //TODO
}
     
void readBytes(int adress, int reg, int length, int *buff){
      int i=0;
      I2C5_start();
      I2C5_write(adress);
      I2C5_write(reg);
      I2C5_restart();
      I2C5_write(adress+1);
      
      while(i<length-1){
                       buff[i]=I2C5_read(0);
                       i+=1;
                       }
      buff[i]=I2C5_read(1);
      I2C5_stop();
      }
void oledWrite(int address, int buff[], int length){
     int i = 0;
     I2C5_start();
     I2C5_write(address);
     while(i<length){
                     I2C5_write(buff[i]);
                     i+=1;
                     }
     I2C5_stop();
     }