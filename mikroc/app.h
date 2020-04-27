


extern volatile unsigned short receiver_on;
extern volatile long receiverx_interrupt, receivery_interrupt,receiverz_interrupt, throttle, throttle_interrupt;
extern volatile int buffer_clear;
extern volatile int receiver_disconnect_counter;
extern volatile char bt;

void initializeTimers();        // Set all timers  1,2,3,4
void initializePorts();         //Initialize ports, input capture
float PID_Calculate(float Kp, float Kd, float Ki, float error, float prev_error, float * integral_value,float min, float max,float dt);   //Calculate PID output
int lcd_output(float line1, float line2, int counter);
void limit_motors(long min, long max);          //Limit mototr outputs between min and max
void calculateAltitude(unsigned long pressure,unsigned long pressure0,long temperature, float *altitude);
void antiwindup(float *integral_value1, float *integral_value2, float *integral_value3,float max_val);              //Prevent integral term from getting too big
void calibrate();            //Angle calibration that goes inside EEprom
void Initialization();       //initialize modules (bno,mpu,uart6,i2c4 &5)