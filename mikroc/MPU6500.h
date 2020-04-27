


void getAccelAndGyro_mpu6500(float * accelAndGyro_mpu6500);    //get accelerometer and gyro values
void MPU6500_init();      //Initialize mpu 6050 (no sleep, filter,etc)
void getOffsets(float *accelAndGyroOffsets_mpu6500);         //Calculate offset values of accel and gyro
void getCompensatedAngleAndGyro(float *accelAndGyroOffsets, float *compensatedAngleAndGyro_mpu6500, float dt);    //calculate yaw pitch roll with complementary filter
 
//(-)compensatedAngleAndGyro[1] is euleurAngles[2] & (-) compensatedAngleAndGyro[0] is eulerAngles[1]        for angles
//(-)compensatedAngleAndGyro[3] is angularVelocities[2] & (-)compensatedAngleAndGyro[3] is angularVelocities[1] & compensatedAngleAndGyro[5] is angularVelocities[0]         for angular velocities