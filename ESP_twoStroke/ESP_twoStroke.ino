#include "ssd1306.h"

//#define PAUSED

int counts = 0;
uint32_t lastUpdate = millis();;

//####################################################//

// for debugging and timing (scope)
void toggle(uint8_t pin){
  pinMode(pin,OUTPUT);
  digitalWrite(pin,!digitalRead(pin));
}

//####################################################//

void pointOnCircle(float rad, int radius, int *x,int *y){
  *x = cos(rad) * radius;
  *y = sin(rad) * radius;
}

//####################################################//

void arrow(int xtip, int ytip, int deg){
  int x,y;
  pointOnCircle(radians(deg), 10, &x, &y);          // point on outer circle  
  ssd1306_drawLine(x+xtip,y+ytip,xtip,ytip);        // draw line from point on circle to center
  pointOnCircle(radians(deg-35), 5, &x, &y);        // point on inner circle  
  ssd1306_drawLine(x+xtip,y+ytip,xtip,ytip);        // draw line from point on circle to center
  pointOnCircle(radians(deg+35), 5, &x, &y);  // point on inner circle  
  ssd1306_drawLine(x+xtip,y+ytip,xtip,ytip);        // draw line from point on circle to center
}

//####################################################//

void twoStrokeDemo(int counts){
  int deg = counts%360;
  int x, y, xc=28, yc=28, radius=18;

  // Draw 2 circles
  for(int deg =0;deg<360;deg++){
    pointOnCircle(radians(deg), radius, &x, &y);
    ssd1306_pixel(x+xc, y+yc);
    pointOnCircle(radians(deg), radius-22, &x, &y);
    ssd1306_pixel(x+xc, y+yc);
  }
  // Draw crankcase
  for(int deg = 90;deg<270;deg++){
    pointOnCircle(radians(deg), radius+5, &x, &y);
    ssd1306_pixel(x+xc, y+yc);
  }
  
  pointOnCircle(radians(deg), radius, &x, &y);  // point on outer circle
  ssd1306_drawLine(x+xc,y+yc,xc,yc);          // draw line from point on circle to center
  ssd1306_drawLine(x+xc,y+yc,x+88,yc);          // draw line from point to piston-pen 
  
  ssd1306_drawLine(x+65,15,x+65,40);            // draw piston bottom
  ssd1306_drawLine(x+65,15,x+101,15);           // draw piston top side
  ssd1306_drawLine(x+65,40,x+101,40);           // draw piston bottom side
  ssd1306_drawLine(x+101,15,x+104,yc);          // draw piston top upper
  ssd1306_drawLine(x+101,40,x+104,yc);          // draw piston top lower
  ssd1306_drawLine(x+97,15,x+97,40);            // draw piston ring 
  ssd1306_drawLine(x+95,15,x+95,40);            // draw piston ring
  
  ssd1306_drawLine(xc,6,60,15);                 // draw crankcase sloping section
  ssd1306_drawLine(60,15,73,15);                // draw cylinder upper 
  ssd1306_drawLine(73,15,73,5);                 // draw cylinder upper
  ssd1306_drawLine(82,15,84,15);                // draw cylinder upper
  ssd1306_drawLine(82,15,82,5);                 // draw cylinder upper
  ssd1306_drawLine(84,15,84,5);                 // draw cylinder upper
  ssd1306_drawLine(95,15,122,15);               // draw cylinder upper
  ssd1306_drawLine(95,15,95,5);                 // draw cylinder upper
  ssd1306_drawLine(60,40,84,40);                // draw cylinder lower
  ssd1306_drawLine(28,50,84,50);                // draw cylinder lower
  ssd1306_drawLine(84,50,92,40);                // draw cylinder lower
  ssd1306_drawLine(92,40,122,40);               // draw cylinder lower
  ssd1306_drawLine(122,16,127,yc);              // draw cylinder head
  ssd1306_drawLine(127,yc,122,40);              // draw cylinder head

  if(deg>=120 && deg <=240){
    arrow(90,3,90);           //exthaust 
    arrow(95,20,30);          //exthaust 
  }
  if((deg>=310 && deg <=359)||(deg>=0 && deg <=60)){
    arrow(77,13,270);         //inlet
  }
  if(deg>=130 && deg <=230){  
    arrow(68,45,180);         //transfer 
    arrow(83,45,180);         //transfer 
    arrow(102,30,150);        //transfer 
  }   
  
  if (deg >= 0)    ssd1306_text8x8(2, 7, "Powerstroke     ");
  if (deg >= 120)  ssd1306_text8x8(2, 7, "Exhaust         ");
  if (deg >= 130)  ssd1306_text8x8(2, 7, "Exhaust/Transfer");
  if (deg >= 240)  ssd1306_text8x8(2, 7, "Exhaust         ");
  if (deg >= 250)  ssd1306_text8x8(2, 7, "Compress        ");
  if (deg >= 300)  ssd1306_text8x8(2, 7, "Compress/Inlett ");
  if (deg >= 340){
    ssd1306_text8x8(120,3,"*");          // draw spark
    ssd1306_text8x8(2, 7, "Ignition       ");
  }

#ifdef PAUSED  
  Serial.print("x = "); Serial.println(x); 
  Serial.print("y = "); Serial.println(y); 
  Serial.print("deg = "); Serial.println(deg); 
#endif
}


//####################################################//

void setup() {
  Serial.begin(115200);
  Serial.printf("\n\n%s\n",__FILE__);
    
  ssd1306_init();//default : sda=4,scl=5,addr=0x3C
  ssd1306_clearBuffer();
  //ssd1306_command(SSD1306_INVERTDISPLAY);
  lastUpdate = millis();
  counts = 0;
}

//####################################################//

void loop(){
  if (millis() - lastUpdate < 200) return; // 5 frames/Sec.(1000/200) 
  lastUpdate = millis(); 

  //------------------------------------
  //90mSec. free to spend on other stuff
  //------------------------------------
  
  toggle(16);
  twoStrokeDemo(counts);   // Processing takes approx 90mSec.
  counts+=10;
  toggle(16);

  ssd1306_displayBuffer();  // I2C takes approx 20mSec. at 400kHz
  ssd1306_clearBuffer();
  
#ifdef PAUSED  
  while(!Serial.available());
  Serial.read();
#endif
}
//####################################################//
    
