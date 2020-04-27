////Written by Samuel Lee
//samfire.pilote@gmail.com

#include "i2c.h"
#include "BNO055.h"
#include "app.h"
#include "MPU6500.h"
#include "error.h"
#include "BMP280.h"
#include "AT24C64.h"






//PID VARIABLES//
volatile float KP1x=8,KD1x=0,KI1x=0;
volatile float KP1y=8, KD1y=0, KI1y=0;             ///////////PID 1///////////
volatile float KP1z=4, KD1z=0, KI1z=0;


volatile float KP2x=6,KD2x=0.55,KI2x=0;
volatile float KP2y=6, KD2y=0.55, KI2y=0;         /////////////PID 2/////////
volatile float KP2z=6, KD2z=0.22, KI2z=0;

volatile float PID1x_output=0, PID1y_output=0, PID1z_output=0;
volatile float PID1x_output_prev=0, PID1y_output_prev=0, PID1z_output_prev=0;
volatile float PID2x_output=0, PID2y_output=0, PID2z_output=0;

volatile float errorx=0, errory=0, errorz=0;
volatile float errorx_prev=0, errory_prev=0, errorz_prev=0;
volatile float error_PID1x=0, error_PID1y=0,error_PID1z=0;
volatile float error_PID1x_prev=0, error_PID1y_prev=0,error_PID1z_prev=0;
volatile float integral1x=0, integral1y=0,integral1z=0;
volatile float integral2x=0,integral2y=0,integral2z=0;
//END OF PID VARIABLES//

//STATE VARIABLES//
volatile float eulerAngles[3],eulerAngles_prev[3], eulerAnglesCalibration[3];
volatile char eulerAnglesCalibrationChar[12];
volatile float angularVelocities[3], angularVelocities_prev[3];
volatile float dt=0.1;

long temperature;
unsigned long pressure, pressure0;

volatile float accelAndGyro_mpu6500[6];
volatile float accelAndGyroOffsets_mpu6500[6];
volatile float eulerAngles_mpu6500[3], eulerAngles_mpu6500_prev[3];
volatile float angularVelocities_mpu6500[3], angularVelocities_mpu6500_prev[3];
volatile float compensatedAngleandGyro[6]={0,0,0,0,0,0};
volatile char bluetooth_read[9]={0,0,0,0,0,0,0,0};
volatile char bluetooth_text_variable[9]={0,0,0,0,0,0,0,0,0};
volatile char eeprom_buff[9]={1,2,3,4,5,6,7,8,9};
volatile float desired_parameter=0;
volatile float desiredx=0,desiredy=0, desiredz=0, desired_angularz=0;
volatile int i=0;
volatile int other_counter=0;
volatile char test2[6] ;
volatile float altitude=0;


volatile float time=0;

volatile int flight_mode=1; //Flight mode 0 : Calibration mode, Flight mode 1 : Normal operation, Flight Mode 2: altitude hold, Flight Mode 3 : GPS hold
//END OF STATE VARIABLES//



//MISCELLANOUS VARIABLES//
volatile int startup_counter=0;
volatile float angle_max=25.0,angular_velocity_max=100.0;

volatile int counter=0;
volatile float var1=0,var2=0,var3=0,var4=0;
volatile char var1s[9],var2s[9],var3s[9],var4s[9],var5s[9],var6s[9],var7s[9],var8s[9],var9s[9],var10s[9],var11s[9],var12s[9];
//END MISCELLANOUS VARIABLES//


//ERROR VARIABLES//

volatile int kill_switchcounter=0;
int toggle=0;
///END ERROR VARIABLES/////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////MAIN PROGRAM/////////////////////////////////////////////////////////////////////////////
void main() {
     ////////////////////////////////////////////////////////INITIALIZATION////////////////////////////////////////////////////////////////////
     Initialization();
     
     getOffsets(&accelAndGyroOffsets_mpu6500[0]);         // Get accel and gyro offset values from mpu6500
     BMP280_begin(MODE_NORMAL, SAMPLING_X16, SAMPLING_X16, FILTER_16, STANDBY_0_5);     //Initialize BMP280
     BMP280_readTemperature(&temperature);                // read temperature
     BMP280_readPressure(&pressure0);                     //read pressure





     ////////////////////////////////////////////////////END INITIALIZATION////////////////////////////////////////////////////////////
     ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



     /////////////////////////////////////////////////////STARTUP PROCEDURE///////////////////////////////////////////////////////////////


     while(startup_counter<15){
           LATESET.B0=1;
           delay_ms(60);
           LATESET.B1=1;
           LATECLR.B0=1;
           delay_ms(60);
           LATESET.B2=1;                          //Leds wave while the motors are starting up (1ms pulse)
           LATECLR.B1=1;
           delay_ms(60);
           LATESET.B3=1;
           LATECLR.B2=1;
           delay_ms(60);
           LATESET.B4=1;
           LATECLR.B3=1;
           delay_ms(60);
           LATECLR.B4=1;
           startup_counter+=1;

           }

     throttle=(throttle_interrupt-20000)/2;
     desiredx=((receiverx_interrupt-30000)*angle_max)/6000;        //   desired angle x between -angle_max and + angle_max
     desiredy=((receivery_interrupt-30000)*angle_max)/6000;        //   desired angle y between -angle_max and + angle_max
     desired_angularz=((receiverz_interrupt-30000)*angular_velocity_max)/8000;                     //   desired angle z between 0 and 360deg
     desiredz=0;

     ///////////////////////////////////////////////////END STARTUP////////////////////////////////////////////////////////////////
     ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     
     //////////////////////////////////////////////////////CALIBRATION//////////////////////////////////////////////////////////////////
     if(receiver_killswitch(throttle,&kill_switchcounter,&toggle)==1 & desiredx<-14){            //Enters when kill_switch activated and right joystick is down
                                                                      flight_mode=0;
                                                                      LATE.B2=1;
                                                                      calibrate();
                                                                      }
     AT24_readByteMultiple(0,50,&eulerAnglesCalibrationChar[0],sizeof(eulerAnglesCalibration));
     eulerAnglesCalibration[0]=*(float*)&eulerAnglesCalibrationChar[0];
     eulerAnglesCalibration[1]=*(float*)&eulerAnglesCalibrationChar[4];
     eulerAnglesCalibration[2]=*(float*)&eulerAnglesCalibrationChar[8];
     //////////////////////////////////////////////////////END CALIBRATION//////////////////////////////////////////////////////////////

     ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     //////////////////////////////////////////////////START CORE LOOP//////////////////////////////////////////////////////////////////
     while(flight_mode!=0){
              //calculate desired angles     by user
              
              throttle=(throttle_interrupt-20000);
              desiredx=((receiverx_interrupt-30000)*angle_max)/6000;        //   desired angle x between -angle_max and + angle_max
              desiredy=((receivery_interrupt-30000)*angle_max)/6000;        //   desired angle y between -angle_max and + angle_max
              desired_angularz=((receiverz_interrupt-30000)*angular_velocity_max)/8000;                     //   desired angle z between 0 and 360deg
              time+=dt;

              if(abs(desiredx)<1){
                                  desiredx=0;
                                  }                         //Compensate for small error in controller so that without asking anything pitch and roll are zero
              if(abs(desiredy)<1){
                                  desiredy=0;
                                  }



              
              
              
              getEulerAngles(&eulerAngles[0]);                        /////retreive compensated angles from BNO055
              
              eulerAngles[1]=eulerAngles[1]-eulerAnglesCalibration[1];
              eulerAngles[2]=eulerAngles[2]-eulerAnglesCalibration[2];       //Subtract calibration data, yaw calibration isn't necessary
              
              getCompensatedAngleAndGyro(&accelAndGyroOffsets_mpu6500[0],&compensatedAngleandGyro[0],dt);      //Get angular velocities from MPU
              BMP280_readTemperature(&temperature);  // read temperature
              BMP280_readPressure(&pressure);
              calculateAltitude(pressure,pressure0,temperature,&altitude);       //calculate altitude

              T4CON.B15=0;
              dt=(float) TMR4;
              dt=(float) ((dt/37500)*15)/1000 ;
              TMR4=0;
              T4CON.B15=1;
              


              errorx=desiredx-eulerAngles[2];
              errory=desiredy-eulerAngles[1];                       //Calculate angle errors                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       insert inside PIDs
              errorz=desiredz-eulerAngles[0];


              //////////////////////////////////////////////////////////YAW CONTROL//////////////////////////////////////////////////////////////////////////
              if (errorz>180){
                 errorz=errorz-360;                                 //Compensate for yaw discontinuity at 0°
                 }
              else if (errorz<-180){
                      errorz=errorz+360;
                      }
              if(abs(desired_angularz)>10){
                                   errorz=0;
                                   PID1z_output=desired_angularz;
                                   desiredz=eulerAngles[0];
                                   }

              ////////////////////////////////////////////////////////END YAW CONTROL////////////////////////////////////////////////////////////////////////

              error_PID1x= -PID1x_output - compensatedAngleAndGyro[3];        //Angular rate error
              error_PID1y= -PID1y_output + compensatedAngleAndGyro[4];
              error_PID1z=  PID1z_output + compensatedAngleAndGyro[5];

              antiwindup(&integral1x,&integral1y,&integral1z,5000);                // Anti wind-up of the first integral controller
              antiwindup(&integral2x,&integral2y,&integral2z,5000);                // Anti wind-up of the second integral controller

              PID1x_output=PID_Calculate(Kp1x,Kd1x,Ki1x,errorx,errorx_prev,&integral1x,-20000,20000,dt);                  //Update pid1 ===> x axis
              PID2x_output=PID_Calculate(Kp2x,Kd2x,Ki2x,error_PID1x,error_PID1x_prev,&integral2x,-20000,20000,dt);         //Update pid2 ====> x axis
              
              PID1y_output=PID_Calculate(Kp1y,Kd1y,Ki1y,errory,errory_prev,&integral1y,-20000,20000,dt);                  //Update pid1 ===> y axis
              PID2y_output=PID_Calculate(Kp2y,Kd2y,Ki2y,error_PID1y,error_PID1y_prev,&integral2y,-20000,20000,dt);         //Update pid2 ====> y axis

              PID1z_output=PID_Calculate(Kp1z,Kd1z,Ki1z,errorz,errorz_prev,&integral1z,-20000,20000,dt);                  //Update pid1 ===> z axis
              PID2z_output=PID_Calculate(Kp2z,Kd2z,Ki2z,error_PID1z,error_PID1z_prev,&integral2z,-20000,20000,dt);         //Update pid2 ====> z axis




              var1=(float) 20000.0 +(float) throttle +(float) PID2x_output*10 +(float) PID2y_output*10 +(float) PID2z_output*10;
              var2=(float) 20000.0 +(float) throttle -(float) PID2x_output*10 +(float) PID2y_output*10 -(float) PID2z_output*10;
              var3=(float) 20000.0 +(float) throttle -(float) PID2x_output*10 -(float) PID2y_output*10 +(float) PID2z_output*10;
              var4=(float) 20000.0 +(float) throttle +(float) PID2x_output*10 -(float) PID2y_output*10 -(float) PID2z_output*10;
              



              OC1RS=(long) var1/2;
              OC2RS=(long) var2/2;
              OC3RS=(long) var3/2;
              OC4RS=(long) var4/2;
              
              limit_motors(10500,19000);            //Max motor speed corresponds to 1.9ms and minimum to 1.1ms
                                                    //Motors must never turn off completely otherwise it takes too much time for them to be at right speed
             
              receiver_killswitch(throttle,&kill_switchcounter,&toggle);         //Kill switch protection
                                   


              eulerAngles_prev[0]=eulerAngles[0];                      //Update old angles and angular velocities
              eulerAngles_prev[1]=eulerAngles[1];
              eulerAngles_prev[2]=eulerAngles[2];

              errorx_prev=errorx;
              errory_prev=errory;            //Update old errors
              errorz_prev=errorz;

              error_PID1x_prev=error_PID1x;
              error_PID1y_prev=error_PID1y;
              error_PID1z_prev=error_PID1z;
              /////////////////////////////////////////////////////BLUETOOTH CONTROL///////////////////////////////////////////////////////////////////////////
              /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
              i=0;
              if(UART6_Data_Ready()==1){
                                      while(UART6_Data_Ready()==1){

                                                                   bluetooth_read[i]=(char) UART6_read();
                                                                   i+=1;
                                                                   }
                                      while(i<=7){
                                                  bluetooth_read[i]=0;
                                                  i+=1;
                                                  }


                                      if(bluetooth_read[0]>=64){
                                                                          //means we are sending text
                                                                          i=0;
                                                                          while(i<=7){
                                                                                      bluetooth_text_variable[i]=bluetooth_read[i];
                                                                                      i+=1;
                                                                                      }

                                                                          }
                                      else{

                                           desired_parameter=atof(&bluetooth_read);
                                           }
                                           }




              if(strcmp(&bluetooth_text_variable,"Kp1x")==0){
                                                             if(desired_parameter!=0){
                                                                                      Kp1x=desired_parameter;
                                                                                      Kp1y=desired_parameter;
                                                                                      }
                                         LATE.B0=1;
                                         }
              else if(strcmp(&bluetooth_text_variable,"Kp2x")==0){
                                                             if(desired_parameter!=0){
                                                                                      Kp2x=desired_parameter;
                                                                                      Kp2y=desired_parameter;
                                                                                      }
                                         LATE.B0=1;
                                         }
              else if(strcmp(&bluetooth_text_variable,"Kd1x")==0){
                                                             if(desired_parameter!=0){
                                                                                      Kd1x=desired_parameter;
                                                                                      Kd1y=desired_parameter;
                                                                                      }
                                         LATE.B0=1;
                                         }
              else if(strcmp(&bluetooth_text_variable,"Kd2x")==0){
                                                             if(desired_parameter!=0){
                                                                                      Kd2x=desired_parameter;
                                                                                      Kd2y=desired_parameter;
                                                                                      }
                                         LATE.B0=1;
                                         }
              else if(strcmp(&bluetooth_text_variable,"Ki1x")==0){
                                                             if(desired_parameter!=0){
                                                                                      Ki1x=desired_parameter;
                                                                                      Ki1y=desired_parameter;
                                                                                      }
                                         LATE.B0=1;
                                         }
              else if(strcmp(&bluetooth_text_variable,"Ki2x")==0){
                                                             if(desired_parameter!=0){
                                                                                      Ki2x=desired_parameter;
                                                                                      Ki2y=desired_parameter;
                                                                                      }
                                         LATE.B0=1;
                                         }
              else if(strcmp(&bluetooth_text_variable,"Kp2z")==0){
                                                             if(desired_parameter!=0){
                                                                                      Kp2z=desired_parameter;
                                                                                      }
                                         LATE.B0=1;
                                         }
              else if(strcmp(&bluetooth_text_variable,"Kd2z")==0){
                                                             if(desired_parameter!=0){
                                                                                      Kd2z=desired_parameter;
                                                                                      }
                                         LATE.B0=1;
                                         }
              else if(strcmp(&bluetooth_text_variable,"Ki2z")==0){
                                                             if(desired_parameter!=0){
                                                                                      Ki2z=desired_parameter;
                                                                                      }
                                         LATE.B0=1;
                                         }
              if(counter>=10){
                              LongIntToHex(time*1000,var1s);
                              LongIntToHex(eulerAngles[2]*1000,var2s);
                              LongIntToHex(eulerAngles[1]*1000,var3s);
                              LongIntToHex(eulerAngles[0]*1000,var4s);
                              LongIntToHex(errorz*1000,var5s);
                              LongIntToHex(error_PID1z*1000,var6s);
                              LongIntToHex(PID2z_output*1000,var7s);
                              LongIntToHex(PID1z_output*1000,var8s);
                              LongIntToHex(desiredz*1000,var9s);
                              LongIntToHex(errorz*1000,var10s);
                              LongIntToHex(eulerAngles[0]*1000,var11s);
                              LongIntToHex(desiredz*1000,var12s);
                              
                              UART6_write_text(var1s);
                              UART6_write_text(var2s);
                              UART6_write_text(var3s);
                              UART6_write_text(var4s);
                              UART6_write_text(var5s);
                              UART6_write_text(var6s);
                              UART6_write_text(var7s);
                              UART6_write_text(var8s);
                              UART6_write_text(var9s);
                              UART6_write_text(var10s);
                              UART6_write_text(var11s);
                              UART6_write_text(var12s);
                              UART6_write_text("\r\n");
                              counter=0;
                              }

              /////////////////////////////////////////////////END BLUETOOTH///////////////////////////////////////////////////////////

              counter+=1;
              //receiver_disconnect_counter+=1;
              //kill_switchcounter+=1;

              if (receiver_disconnect_counter>40){
                 receiver_disconnect(&receiver_disconnect_counter);
                 errorx=0;
                 errorx_prev=0;
                 error_PID1x=0;
                 error_PID1x_prev=0;
                 errory=0;
                 errory_prev=0;
                 error_PID1y=0;
                 error_PID1y_prev=0;
                 errorz=0;
                 errorz_prev=0;
                 error_PID1z=0;
                 error_PID1z_prev=0;
                 }
              delay_ms(1);
              
              //counter=lcd_output(eulerAngles[1],eulerAngles[2],counter);
              }

      //////////////////////////////////////////////////END CORE LOOP///////////////////////////////////////////////////////////////////
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}