# ESP8266  
**A collection of simple tryouts and/or examples**  

####ESP_AI_Thinker_Web_Command_table  
>   A simple webserver for the AI-Thinker development board  
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
>   The ESP I used is a nudemcu 1.0 dev.board 
>   The input-ranges are send from the WS-client to the WS-server to provide some trace settings     
>   See screenshot of a canvas , displaying a musical cord.  
![](https://github.com/jgmbrand/ESP8266/blob/master/ESP_WS_binary_data/Doc/Cord.jpg)
