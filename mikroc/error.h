


void receiver_disconnect(int * receiver_disconnect_counter);    //Turns off motors if controller signal is lost during 5 cycles and 5 Leds blink rapidly

int receiver_killswitch(float throttle, int *counter, int *toggle); //Disables outputs if killswitch is turned & leds blink with a 2-3 pattern

void i2c_error();   //I2C error