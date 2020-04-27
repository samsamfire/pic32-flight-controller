
#include "MPU_9250.h"
#include "i2c.h"

void mpu_init(){
     writeByte(MPU9250_ADDRESS,PWR_MGMT_1,0x00);  //Disable sleep mode
     delay_ms(100);               //Wait for PLL
     writeByte(MPU9250_ADDRESS,PWR_MGMT_1,0x01); //Set clock source
     
     writeByte(MPU9250_ADDRESS,CONFIG,0x03); //DLPF set to 41Hz
     
     
     writeByte(MPU9250_ADDRESS, SMPLRT_DIV, 0x04); //Sample rate 200Hz
     }
     
void getAccel(int *accel) {
     readBytes(MPU9250_ADDRESS,ACCEL_XOUT_H,6,accel);
     }
     
void getGyro(int *gyro){
     readBytes(MPU9250_ADDRESS,GYRO_XOUT_H,6,gyro);
     }