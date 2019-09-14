/////////////////////////////
// low level I2C functions 
/////////////////////////////
// Clock speed approx 435khz
// Extra on SCL pullup might be needed
////////////////////////////////////////

#include "i2c.h"

uint8_t i2c_dcount = 1;           // this sets bus speed to 435kHz(increase to slow down)
static uint8_t i2c_sda, i2c_scl;

#define SDA_LOW()   (GPES = (1 << i2c_sda)) //Enable SDA (becomes output and since GPO is 0 for the pin, it will pull the line low)
#define SDA_HIGH()  (GPEC = (1 << i2c_sda)) //Disable SDA (becomes input and since it has pullup it will go high)
#define SDA_READ()  ((GPI & (1 << i2c_sda)) != 0)
#define SCL_LOW()   (GPES = (1 << i2c_scl))
#define SCL_HIGH()  (GPEC = (1 << i2c_scl))
#define SCL_READ()  ((GPI & (1 << i2c_scl)) != 0)

//////////////////////
// local functions  //
//////////////////////

/**************************/
/* delay some nanoSeconds */
/**************************/
static void i2c_delay(uint8_t v){
  uint8_t i, reg;
  for(i=0;i<v;i++) reg = GPI;
}

/**************************/
/* writes/clocks one bit out */ 
/**************************/
static bool i2c_write_bit(bool bit) {
  uint32_t i = 0;
  SCL_LOW();
  bit?SDA_HIGH():SDA_LOW();
  i2c_delay(i2c_dcount+1);
  SCL_HIGH();
  i2c_delay(i2c_dcount);
  return true;
}

/**************************/
/* reads/clocks one bit in */ 
/**************************/
static bool i2c_read_bit(void) {
  uint32_t i = 0;
  SCL_LOW();
  SDA_HIGH();
  i2c_delay(i2c_dcount+2);
  SCL_HIGH();
  bool bit = SDA_READ();
  i2c_delay(i2c_dcount);
  return bit;
}

///////////////////////
// global functions  //
///////////////////////

/**************************/
/* initialize I2C bus     */
/**************************/
void i2c_init(uint8_t sda, uint8_t scl){
  i2c_sda = sda;
  i2c_scl = scl;
  pinMode(i2c_sda, INPUT_PULLUP);
  pinMode(i2c_scl, INPUT_PULLUP);
}

/**************************/
/* set idle I2C bus       */
/**************************/
void _i2c_stop(void){
  pinMode(i2c_sda, INPUT);
  pinMode(i2c_scl, INPUT);
}

/******************************/
/* create I2C START condition */
/******************************/
bool i2c_write_start(void) {
  SCL_HIGH();
  SDA_HIGH();
  if (SDA_READ() == 0) return false;
  i2c_delay(i2c_dcount);
  SDA_LOW();
  i2c_delay(i2c_dcount);
  return true;
}

/*****************************/
/* create I2C STOP condition */
/*****************************/
bool i2c_write_stop(void){
  uint32_t i = 0;
  SCL_LOW();
  SDA_LOW();
  i2c_delay(i2c_dcount);
  SCL_HIGH();
  i2c_delay(i2c_dcount);
  SDA_HIGH();
  i2c_delay(i2c_dcount);
  return true;
}

/**************************/
/* write one single byte  */
/**************************/
bool i2c_write_byte(unsigned char byte) {
  unsigned char bit;
  for (bit = 0; bit < 8; bit++) {
    i2c_write_bit(byte & 0x80);
    byte <<= 1;
  }
  return !i2c_read_bit();//NACK/ACK
}

/**************************/
/* read one single byte   */
/**************************/
uint8_t i2c_read_byte(bool nack) {
  uint8_t inByte = 0;
  uint8_t inBit;
  for (inBit = 0; inBit < 8; inBit++) inByte = (inByte << 1) | i2c_read_bit();
  i2c_write_bit(nack);
  return inByte;
}
