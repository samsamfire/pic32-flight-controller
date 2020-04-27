

void AT24_writeByte(int reg_high, int reg_low, int value);                       //Write single byte to at24c64



char AT24_readByte(int reg_high, int reg_low);                                     //Read single byte

void AT24_writeByteMultiple(int reg_high, int reg_low, char *eeprom_buff,int len);        //len = number of bytes to write

char AT24_readByteMultiple(int reg_high, int reg_low,char *eeprom_buff, int len);          //len= number of bytes to read