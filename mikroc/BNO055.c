
#include "i2c.h"

#define OPR_MODE 0x3D
#define UNIT_SEL 0x3B
#define BNO055_ADDRESS 0x50
#define PWR_MODE 0x3E
#define SYS_TRIGGER 0x3F


void Bno055_init(){
     writeByte(BNO055_ADDRESS,OPR_MODE,0x00);
     delay_ms(200);
     writeByte(BNO055_ADDRESS,SYS_TRIGGER,0x20);
     delay_ms(680);
     writeByte(BNO055_ADDRESS,UNIT_SEL,0x00);
     delay_ms(200);
     writeByte(BNO055_ADDRESS,PWR_MODE,0x00);
     delay_ms(200);
     writeByte(BNO055_ADDRESS,SYS_TRIGGER,0x00);
     delay_ms(200);
     writeByte(BNO055_ADDRESS,OPR_MODE,0b00001100);
     delay_ms(100);


     }

void getEulerAngles(float * eulerAngles){
     int buff[6];
     int heading=0, roll=0, pitch=0;
     readBytes(BNO055_ADDRESS,0x1A,6,&buff[0]);
     heading=(int)buff[0]+256*(int)buff[1];   //Heading = Z axis
     eulerAngles[0]=(float)heading/16;
     roll=(int)buff[2]+256*(int)buff[3];      //Roll = Y axis
     eulerAngles[1]=(float)roll/16;
     pitch=(int)buff[4]+256*(int)buff[5];     //Pitch = X axis
     eulerAngles[2]=(float)pitch/16;

     }
void getAngularVelocities(float * angularVelocities){
     int buff[6];
     int gyrox=0,gyroy=0,gyroz=0;
     readBytes(BNO055_ADDRESS,0x14,6,&buff[0]);
     gyrox=(int)buff[0]+256*(int)buff[1];      //Pitch
     angularVelocities[2]=(float)gyrox/16;
     gyroy=(int)buff[2]+256*(int)buff[3];      //Roll
     angularVelocities[1]=(float)gyroy/16;
     gyroz=(int)buff[4]+256*(int)buff[5];      //Heading
     angularVelocities[0]=(float)gyroz/16;
     
     }