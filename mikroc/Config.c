/*//Microcontroller Configuration Functions

#include "Config.h"



void init_ports(){
        ANSELA=0;
        ANSELB=0;

        TRISB.B4=1;          //IC 4 as input
        TRISA.B2=1;
        TRISA.B3=1;
        TRISA.B4=1;
        TRISB.B5=1;

        TRISB.B12=0;

        TRISB.B2=0;
        TRISB.B3=0;           //i2c
        CNPUB.B2=1;
        CNPUB.B3=1;

        TRISB.B11=0;
        TRISB.B13=0;
        TRISB.B9=0;            //OC as outputs
        TRISB.B15=0;
        LATB=0x0006;
        RPB11R=0b0101;
        RPB15R=0b0101;          //OC1
        RPB13R=0b0101;
        RPB9R=0b0101;//RPB14R=0b0101;          //OC3         //OC3
                         //input capture init PPS
        IC2R=0b0000;
        IC5R=0b0000;
        IC4R=0b0010;
        IC1R=0b0010;
        IC3R=0b0001;

        }
void icconfig(){
     IC4CON=0b1000000000000001;
     delay_us(1);
     IEC0.IC4IE=1;      //IC 4 enable interrupt
     IPC4.B10=1; //Priority 7
     IPC4.B11=1;
     IPC4.B12=1;
     IFS0.IC4IF=0;     //interrupt flag*/

     IC5CON=0b1000000000000001;
     delay_us(1);
     IEC0.IC5IE=1;      //IC 5 enable interrupt
     IPC5.B10=1; //Priority 7
     IPC5.B11=1;
     IPC5.B12=1;
     IFS0.IC5IF=0;     //interrupt flag*/

     IC2CON=0b1000000000000001;
     delay_us(1);
     IEC0.IC2IE=1;      //IC 2 enable interrupt
     IPC2.B10=1; //Priority 7
     IPC2.B11=1;
     IPC2.B12=1;
     IFS0.IC2IF=0;     //interrupt flag*/

     IC1CON=0b1000000000000001;
     delay_us(1);
     IEC0.IC1IE=1;      //IC 1 enable interrupt
     IPC1.B10=1; //Priority 7
     IPC1.B11=1;
     IPC1.B12=1;
     IFS0.IC1IF=0;     //interrupt flag*/

     IC3CON=0b1000000000000001;
     delay_us(1);
     IEC0.IC3IE=1;      //IC 3 enable interrupt
     IPC3.B10=1; //Priority 7
     IPC3.B11=1;                                   //use timer 2 as timer for altimeter
     IPC3.B12=1;
     IFS0.IC3IF=0;     //interrupt flag*/



     }
void    init_mpu_6050() {
//mpu 6050 initialization.
      I2C2_init_advanced(10000,700000);
      I2C2_start();
      I2C2_write(0xD0);
      I2C2_write(107); //sleep mode reg
      I2C2_write(0); // disable sleep mode
      I2C2_stop();
      }
void setDigitalFilter(){
     I2C2_start();
     I2C2_write(0xD0);
     I2C2_write(26);
     I2C2_write(0b00000001);
     I2C2_stop();
     }
void setGyro(){
     I2C2_start();
     I2C2_write(0xD0);
     I2C2_write(27);
     I2C2_write(0b00001000);
     I2C2_stop();
     }
void setAccel(){
     I2C2_start();
     I2C2_write(0xD0);
     I2C2_write(28);
     I2C2_write(0b00010000);
     I2C2_stop();
     }
void InitTimer1234(){
     T2CON=0x8020;
     T2CON.B15=0;
     PR2=40000;
     TMR2=0;

     T4CON=0x8020;
     T4CON.B15=0;
     PR4=40000;
     TMR4=0;

     T3CON=0x8020;
     T3CON.B15=1;
     PR3=40000;
     TMR3=0;

     T1CON=0x8010;
     T1CON.B15=0;
     PR1=50000;
     TMR1=0;
}
void getOffsets(long ax_off, long ay_off, long az_off, long gx_off, long gy_off, long gz_off, long temp_off){
     char read_h=0;
     char read_l=0;
     int read_mpu=0;
     I2C2_start();
     I2C2_write(0xD0);
     I2C2_write(59);
     I2C2_restart();
     I2C2_write(0xD1);

     read_h=I2C2_read(0);
     read_l=I2C2_read(0);
     ax_off=256*read_h+read_l;
     read_h=I2C2_read(0);
     read_l=I2C2_read(0);
     ay_off=256*read_h+read_l;
     read_h=I2C2_read(0);
     read_l=I2C2_read(0);
     az_off=256*read_h+read_l;
     read_h=I2C2_read(0);
     read_l=I2C2_read(0);
     temp_off=256*read_h+read_l;
     read_h=I2C2_read(0);
     read_l=I2C2_read(0);
     gx_off=256*read_h+read_l;
     read_h=I2C2_read(0);
     read_l=I2C2_read(0);
     gy_off=256*read_h+read_l;
     read_h=I2C2_read(0);
     read_l=I2C2_read(1);
     gz_off=256*read_h+read_l;
     I2C2_stop();
     }
void sendAltitudeRequest(){
     LATB.B12=1;
     delay_us(20);
     LATB.B12=0;
     }*/