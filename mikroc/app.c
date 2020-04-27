
#include "AT24C64.h"
#include "i2c.h"
#include "MPU6500.h"
#include "error.h"
//#include "BMP280.h"
#include "BNO055.h"
static float eulerAnglesCalibrate[3];
//RECEIVER INTERRUPT VARIABLES//
volatile unsigned short receiver_on=1;
volatile int receiver_disconnect_counter=0;
volatile long receiverx_interrupt=30000, receivery_interrupt=30000,receiverz_interrupt=30000, throttle=0, throttle_interrupt=20000;
volatile buffer_clear=0;
//END OF RECEIVER VARIABLES//
volatile char bt=0;

//////////////////////////////////////////////////INTERRUPTS/////////////////////////////////////////////////////////////////////////////////

void Timer1Interrupt() iv IVT_TIMER_1 ilevel 7 ics ICS_SRS {

  T1IF_bit         = 0;
  //LATE.B0=1;
  T1CON.B15=0;
  TMR1=0;
  T1CON.B15=1;


}
void Timer2Interrupt() iv IVT_TIMER_2 ilevel 7 ics ICS_SRS {

     T2IF_bit=0;
}
void input_capture1() iv IVT_INPUT_CAPTURE_1 ilevel 7 ics ICS_SOFT{
     if (PORTD.B8==1){
        T3CON.B15=0;
        TMR3=0;
        T3CON.B15=1;
        buffer_clear=IC1BUF;
        }
     else if(PORTD.B8==0){
          receivery_interrupt=IC1BUF;
          receiver_on=1;
          }
     IFS0.IC1IF=0;  //interrupt flag
}

void input_capture2() iv IVT_INPUT_CAPTURE_2 ilevel 7 ics ICS_SOFT{
     if (PORTD.B9==1){
        T3CON.B15=0;
        TMR3=0;
        T3CON.B15=1;
        buffer_clear=IC2BUF;
        }
     else if(PORTD.B9==0){
          receiverx_interrupt=IC2BUF;
          }
     IFS0.IC2IF=0;  //interrupt flag
     receiver_disconnect_counter=0;
}

void input_capture3() iv IVT_INPUT_CAPTURE_3 ilevel 7 ics ICS_SOFT{
     if (PORTD.B10==1){
        T3CON.B15=0;
        TMR3=0;
        T3CON.B15=1;
        buffer_clear=IC3BUF;
        }
     else if(PORTD.B10==0){
          throttle_interrupt=IC3BUF;
          }
     IFS0.IC3IF=0;  //interrupt flag
}

void input_capture4() iv IVT_INPUT_CAPTURE_4 ilevel 7 ics ICS_SOFT{
     if (PORTD.B11==1){
        T3CON.B15=0;
        TMR3=0;
        T3CON.B15=1;
        buffer_clear=IC4BUF;
        }
     else if(PORTD.B11==0){
          receiverz_interrupt=IC4BUF;
          }
     IFS0.IC4IF=0;  //interrupt flag
}


void input_capture5() iv IVT_INPUT_CAPTURE_5 ilevel 7 ics ICS_SOFT{
     if (PORTD.B4==1){
        T3CON.B15=0;
        TMR3=0;
        T3CON.B15=1;
        buffer_clear=IC5BUF;
        }
     else if(PORTD.B4==0){
          buffer_clear=IC5BUF;
          }
     IFS0.IC5IF=0;  //interrupt flag
}


void bluetooth_input() iv IVT_UART_6 ilevel 6 ics ICS_SOFT {
     bt=UART6_read();

     PORTE.B0=1;

     IFS2.U6RXIF=0;

}

//////////////////////////////////////////////////////END INTERRUPTS/////////////////////////////////////////////////////////////////////////////


void initializeTimers(){


     
     T3CON=0x8020;
     T3CON.B15=0;
     PR3=41000;               ////interrupt every 2.05ms ==> 20 000 increments =1ms (interupt deactivated)
     TMR3=0;
     
     
    T4CON         = 0x8050;
    T4CON.B15=0;                           //interrupt every 15ms
    PR4                 = 37500;
    TMR4                 = 0;


     
     
     T1CON         = 0x8010;
     T1IP0_bit         = 1;
     T1IP1_bit         = 1;
     T1IP2_bit         = 1;
     T1IF_bit         = 0;
     T1IE_bit         = 1;
     PR1                 = 25000;
     TMR1                 = 0;



     T1CON.B15=1;                     //start timer 1
     
     }
     
void initializePorts(){

     TRISECLR=0xFFFF;
     LATE.B7=0;
     LATECLR=0xFFFF;
     //TRISDCLR=0xFFFF;          //PORT D INIT NOT MANDATORY
     LATDCLR=0xFFFF;
     
     //INPUT CAPTURE CONFIG//

     IC4CON=0b1000000000000001;

     IEC0.IC4IE=1;      //IC 4 enable interrupt
     IEC0.B17=1;
     IPC4.B10=1; //Priority 7
     IPC4.B11=1;
     IPC4.B12=1;
     IFS0.IC4IF=0;     //interrupt flag

     IC5CON=0b1000000000000001;

     IEC0.IC5IE=1;      //IC 5 enable interrupt
     IPC5.B10=1; //Priority 7
     IPC5.B11=1;
     IPC5.B12=1;
     IFS0.IC5IF=0;     //interrupt flag

     IC2CON=0b1000000000000001;

     IEC0.IC2IE=1;      //IC 2 enable interrupt
     IPC2.B10=1; //Priority 7
     IPC2.B11=1;
     IPC2.B12=1;
     IFS0.IC2IF=0;     //interrupt flag

     IC1CON=0b1000000000000001;

     IEC0.IC1IE=1;      //IC 1 enable interrupt
     IPC1.B10=1; //Priority 7
     IPC1.B11=1;
     IPC1.B12=1;
     IFS0.IC1IF=0;     //interrupt flag

     IC3CON=0b1000000000000001;

     IEC0.IC3IE=1;      //IC 3 enable interrupt
     IPC3.B10=1; //Priority 7
     IPC3.B11=1;                                   //use timer 2 as timer for altimeter
     IPC3.B12=1;
     IFS0.IC3IF=0;     //interrupt flag
     
          OC1CON = 0x0000; // Turn off the OC1 when performing the setup
     OC1R = 0x0064; // Initialize primary Compare register
     OC1RS = 10000; // Initialize secondary Compare register
     OC1CON = 0x0006; // Configure for PWM mode without Fault pin

     OC2CON = 0x0000; // Turn off the OC1 when performing the setup
     OC2R = 0x0064; // Initialize primary Compare register
     OC2RS = 10000; // Initialize secondary Compare register
     OC2CON = 0x0006; // Configure for PWM mode without Fault pin

     OC3CON = 0x0000; // Turn off the OC1 when performing the setup
     OC3R = 0x0064; // Initialize primary Compare register
     OC3RS = 10000; // Initialize secondary Compare register
     OC3CON = 0x0006; // Configure for PWM mode without Fault pin

     OC4CON = 0x0000; // Turn off the OC1 when performing the setup
     OC4R = 0x0064; // Initialize primary Compare register
     OC4RS = 10000; // Initialize secondary Compare register
     OC4CON = 0x0006; // Configure for PWM mode without Fault pin





     // enabled
     PR2 = 25000; // Set period
     // Configure Timer2 interrupt. Note that in PWM mode, the
     // corresponding source timer interrupt flag is asserted.
     // OC interrupt is not generated in PWM mode.
     IFS0CLR = 0x00000100; // Clear the T2 interrupt flag
     IEC0SET = 0x00000100; // Enable T2 interrupt
     IPC2SET = 0x0000001C; // Set T2 interrupt priority to 7
     T2CONSET = 0x8030; // Enable Timer2


     OC1CONSET = 0x8000; // Enable OC1
     OC2CONSET = 0x8000; // Enable OC2
     OC3CONSET = 0x8000; // Enable OC3
     OC4CONSET = 0x8000; // Enable OC4


     }
     
     
float PID_Calculate(float Kp, float Kd, float Ki, float error,float prev_error, float * integral_value, float min, float max,float dt){
     float proportional=0;
     float derivative=0;
     float result=0;
     proportional=error*Kp;
     derivative=(float)((error-prev_error)*Kd)/dt;
     integral_value[0] = Ki*error*dt + integral_value[0];
     result = proportional+derivative+integral_value[0];
     if(result>max){
                    return max;
                    }
     else if(result<min){
          return min;
          }
     else
         return result;


}
int lcd_output(float line1, float line2, int counter){
       char out1[16], out2[16];
       floattostr(line1,out1);
       floattostr(line2,out2);


       if (counter>=40){
                        Lcd_Cmd(_LCD_CLEAR);               // Clear display
                        Lcd_Out(1,1,out1);
                        Lcd_Out(2,1,out2);
                        counter=0;}

       return counter ;
       }
       
void limit_motors(long min1, long max1){
      OC1RS=(long) min(OC1RS,max1);
      OC1RS=(long) max(OC1RS,min1);
      OC2RS=(long) min(OC2RS,max1);
      OC2RS=(long) max(OC2RS,min1);
      OC3RS=(long) min(OC3RS,max1);                      //Limit motor outputs between 1ms and 1.9ms
      OC3RS=(long) max(OC3RS,min1);
      OC4RS=(long) min(OC4RS,max1);
      OC4RS=(long) max(OC4RS,min1);
     }
     
void calculateAltitude(unsigned long pressure,unsigned long pressure0,long temperature, float *altitude){
     float a=0;
     a=(float)((float) pressure0/ (float) pressure);
     a=(float)(pow(a,1/5.257)-1);
     a=a*((float) temperature/100 +273.15);
     a=a/0.0065;
     *altitude=a;

     }
     
void antiwindup(float * integral_value1, float * integral_value2, float * integral_value3, float max_val){
     if (abs(integral_value1[0])>=max_val){
        if(integral_value1[0]<0){
                              integral_value1[0]=-max_val;
                              }
        else integral_value1[0]=max_val;
        }
     if (abs(integral_value2[0])>=max_val){
        if(integral_value2[0]<0){
                              integral_value2[0]=-max_val;
                              }
        else integral_value2[0]=max_val;
        }
     if (abs(integral_value3[0])>=max_val){
        if(integral_value3[0]<0){
                              integral_value3[0]=-max_val;
                              }
        else integral_value3[0]=max_val;
        }
     }
     
void calibrate(){

     getEulerAngles(&eulerAnglesCalibrate[0]);
     I2C_Set_Active(&I2C4_Start, &I2C4_Restart, &I2C4_Read, &I2C4_Write, &I2C4_Stop, &I2C4_Is_Idle); // Sets the I2C4 module active
     AT24_writeByteMultiple(0,50,&eulerAnglesCalibrate[0],sizeof(eulerAnglesCalibrate));
     delay_ms(15);

}

void Initialization(){
     CHECON = 0x32;
     AD1PCFG=0xFFFF;            //ALL ports are digital
     DDPCON=0x00;              //Disable JTAG
     delay_ms(100);

     initializeTimers();                  //Initialize timers
     initializePorts();                   //Initialize Ports


     I2C5_Init_Advanced(80000,800000);     //Start I2c5
     delay_ms(400);

     Bno055_init();                       //Initialize Bno055
     MPU6500_init();                      //Initialize MPU6500
     //BMP280_begin(MODE_NORMAL, SAMPLING_X16, SAMPLING_X16, FILTER_16, STANDBY_0_5);     //Initialize BMP280
     delay_ms(100);


     EnableInterrupts();
     UART6_Init(921600);
     delay_ms(100);
     U6MODE.BRGH=1;                      //Initialize Bluetooth
     U6BRG=21;
     delay_ms(1000);
     I2C4_Init_Advanced(80000,800000);        //Start I2c4
     }