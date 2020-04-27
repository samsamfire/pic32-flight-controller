#include "Attitude.h"
float gRes = 0.00762939;



void  updateAccelAngle(float ax, float ay, float az, float * angleAccel){
      angleAccel[0]=(float) atan(ax/sqrt(ay*ay+az*az))*180/3.14;
      angleAccel[1]=(float) atan(ay/sqrt(ax*ax+az*az))*180/3.14;
      }
      
void updateGyroAngle(float gx, float gy, float gz, float * angleGyro, float deltat){
     angleGyro[0]-= (float) gy*gRes*deltat;
     angleGyro[1]+= (float) gx*gRes*deltat;
     angleGyro[2]-= (float) gz*gRes*deltat;
     }

void complementaryFilter(float * angleAccel, float gx,float gy, float gz, float * angleFiltered, float a,float deltat){
     angleFiltered[0]=(float)(1-a)*(angleFiltered[0]-(float) gy*gRes*deltat)+ a*angleAccel[0];
     angleFiltered[1]=(float)(1-a)*(angleFiltered[1]+ (float) gx*gRes*deltat)+ a*angleAccel[1];
     }