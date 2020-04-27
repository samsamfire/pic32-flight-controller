

void writeByte(int address, int reg, int value);         //Write single byte  via i2c
char  readByte(int adress, int reg);                      //Read single byte

void i2c_write_multiple(int adress, int reg, int buff[], int length);   //Write multiple bytes
void readBytes(int adress, int reg,int length, int *buff);              //Read multiple bytes
void oledWrite(int address, int buff[], int length);                 //Writing via i2c to oled display