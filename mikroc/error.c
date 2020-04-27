
#include "app.h"
void receiver_disconnect(int * receiver_disconnect_counter){
     OC1RS=10000;
     OC2RS=10000;        //Motors off
     OC3RS=10000;
     OC4RS=10000;
     receiverx_interrupt=30000;
     receivery_interrupt=30000;
     receiverz_interrupt=30000; 
     throttle=0;
     throttle_interrupt=20000;
     while( *receiver_disconnect_counter > 5){
     LATE=0xFFFF;
     delay_ms(200);
     LATE=0;
     delay_ms(200);
     }
}

int receiver_killswitch(float throttle, int *counter, int *toggle){
     if(abs(throttle)<1000){                //Means kill switch pressed
       OC1RS=10000;
       OC2RS=10000;        //Motors off
       OC3RS=10000;
       OC4RS=10000;
       if(*counter>=30 & *toggle==1){
                       LATE.B0=1;
                       LATE.B4=1;
                       LATE.B1=0;
                       LATE.B2=0;
                       LATE.B3=0;
                       *toggle=0;
                       *counter=0;
                       }
       else if(*counter>=30 & *toggle==0)  {
                           LATE.B0=0;
                           LATE.B4=0;
                           LATE.B1=1;
                           LATE.B2=1;
                           LATE.B3=1;
                           *toggle=1;
                           *counter=0;
                           }
       return 1;
       }
     return 0 ;
     }