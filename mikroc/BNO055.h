


void Bno055_init();     //Initialize Bno055 euler angles output mode (9 axis)

void getEulerAngles(float * eulerAngles);       // Get euler angles, eulerAngles=[yaw (Z), Roll (Y), Pitch (X)]

void getAngularVelocities(float * angularVelocities);   //Get angular velocities angularVelocities=[yaw (Z), Roll (Y), Pitch (X)] /dt