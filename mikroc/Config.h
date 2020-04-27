#ifndef CONFIG_H
#define CONFIG_H






void getAccelandGyro(long ax, long ay, long az, long gx, long gy, long gz, long temp);
void init_ports();
void icconfig();
void init_mpu_6050();
void setDigitalFilter();
void setGyro();
void setAccel();
void InitTimer1234();
void getOffsets(long ax_off, long ay_off, long az_off, long gx_off, long gy_off, long gz_off, long temp_off);
void sendAltitudeRequest();

#endif