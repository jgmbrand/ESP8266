# ESP8266  
**A collection of simple tryouts and/or examples**  

####ESP_AI_Thinker_Web_Command_table  
>   A simple webserver for the AI-Thinker development board  
>   AI Thinker Board  

![AI Thinker Board](https://github.com/jgmbrand/ESP8266/blob/master/ESP_AI_Thinker_Web_Command_table/Doc/AI_Thinker_Development_board.jpg)  

>   Use a webbrowser to control all onboard LEDS  

![AI Thinker Board](https://github.com/jgmbrand/ESP8266/blob/master/ESP_AI_Thinker_Web_Command_table/Doc/WebPage.jpg)  
  
  
####ESP_mqtt_Adafruit_io

>   MQTT tryout with io.adafruit.com dashboards  
>	  GPIO subscriptions  
>	  Sensor publications  
>	  Dashboard examples in "Doc" folder  
>	  Example on how to send your location data  

![](https://github.com/jgmbrand/ESP8266/blob/master/ESP_mqtt_Adafruit_io/Doc/Adafruit_MQTT_Dashboard.jpg)


####ESP_WS_binary_data

>   Tryout of a Websocket-server sending binary data  
>   Websocket-client "canvasTrace.html" is in "Doc" folder (use correct IP address : see serial monitor)  
>   Audio is sampled by the ADC input(0-3v3) and send to the websocket client "canvasTrace.html"  
>   There is some very simple code to provide some trigger level  
>   Audio source is a $1.50 mp3 player from Aliexpress(link below)   
>   http://www.aliexpress.com/item/1pcs-TF-card-U-disk-MP3-Format-decoder-board-module-amplifier-decoding-audio-Player/32622813538.html  
>   There is a simple diagram to create a 500mV offset of the trace  
>   The ESP8266 used is a nodemcu 1.0 dev.board  
>   The WS-client has 2 input-ranges who's values are send to the WS-server to provide some trace settings       
>   See screenshot of a canvas , displaying a musical cord. (Chrome)   
![](https://github.com/jgmbrand/ESP8266/blob/master/ESP_WS_binary_data/Doc/Cord.jpg)

####ESP_twoStroke

>   SSD1306 Oled display tryouts.  
>   Animated simulation of a twostroke engine.  
>   Mainly to exercise pixels, lines, circles, text  etc.  
>   i2c and ssd1306 code is partly based on (ripped from) existing libraries.  
>   Because of the 400kHz i2c clock speed an extra pullup might be required.(e.g. 2k2 ohm)  
![](https://github.com/jgmbrand/ESP8266/blob/master/ESP_twoStroke/Doc/screen.jpg)

