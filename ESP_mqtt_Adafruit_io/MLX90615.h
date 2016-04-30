
//temperature measuring types
#define OBJECT_TEMPERATURE      0x27
#define AMBIENT_TEMPERATURE     0x26

//ESP-01 I2C pins
#define SDA_01  2
#define SCL_01  0

I2C_BB IIC(SDA_01,SCL_01); // create I2C_BB object IIC with sda,scl pins

/////////////////////////////////////////////////////
// Get temperature type (object/ambient) from MLX90615 
float mlx90615_Temperature(uint8_t type){  
   
  IIC.start();                          //start (transmission begin)  
  IIC.write(0xB6);                      //write device address for WRITE
  IIC.write(type);                      //write temperature type (object/ambient) 
  IIC.start();                          //repeated start
  IIC.write(0xB7);                      //write device address for READ
  uint8_t dataLow = IIC.read(true);     //true means give ACK (sda = LOW)
  uint8_t dataHigh = IIC.read(true);    //true equals give ACK (sda = LOW)
  uint8_t pec = IIC.read(false);        //false equals give NOT ACK (sda = HIGH)
  IIC.stop();                           //stop (transmission end)
  //Serial.println(dataLow,HEX);
  //Serial.println(dataHigh,HEX);
  //Serial.println(pec,HEX);
  //Serial.println("###################");

  //convert received bytes to temperature, MSB is a error bit and is ignored for temperature readings
  // (raw data * degrees per LSB) - absolute zero
  float celcius = ((float)(((dataHigh & 0x7F) << 8) + dataLow) * 0.02) - 273.15;
  float fahrenheit = (celcius*1.8) + 32;
 
  return(celcius); 
}
