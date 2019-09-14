/////////////////////////////
// low level I2C functions //
/////////////////////////////

#ifndef I2C_h
#define I2C_h
#include "Arduino.h"

void i2c_init(uint8_t sda, uint8_t scl);
bool i2c_write_start(void);
bool i2c_write_byte(uint8_t);
bool i2c_write_stop(void);


uint8_t i2c_read_byte(bool);
void i2c_stop(void);

#endif
