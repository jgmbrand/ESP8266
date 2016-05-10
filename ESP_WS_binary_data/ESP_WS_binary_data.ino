/*
 * WebSocketServer Fast update tryout
 *
 *  Created on: 03-05-2016
 *  
 */

#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include "credentials.h"

WebSocketsServer webSocket = WebSocketsServer(81);

int sampleTime = 50;
int triggerLevel = 500;
int nrSamples = 513;
uint8_t trace[520];
boolean hasClient = false;

/****************************************************************************************************/

void setupWiFi_STA(){
  const char* ssid      = YOUR_SSID;    //define this in credentials.h
  const char* password  = YOUR_PASS;    //define this in credentials.h
  Serial.println();Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
/****************************************************************************************************/

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t nrBytes) {
  switch(type) {
    
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      hasClient = false;
    break;
    
    case WStype_CONNECTED:{ 
      IPAddress ip = webSocket.remoteIP(num);
      Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      hasClient = true;
      //send message to client
      //webSocket.sendTXT(num, "Connected");
    }  
    break;
    
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s : %u\n", num, payload, nrBytes);
      if (payload[0] == 's') sampleTime   = atoi((char *)&payload[1]);
      if (payload[0] == 't') triggerLevel = atoi((char *)&payload[1]);
      Serial.print("sample  time  : ");Serial.println(sampleTime,DEC);
      Serial.print("trigger level : ");Serial.println(triggerLevel,DEC);
 
      //send message to client
      //webSocket.sendTXT(num, payload);
    break;
  }
}
/****************************************************************************************************/

void setup() {
    Serial.begin(115200);
    Serial.printf("\r\n\n\n");
    Serial.println(__FILE__);

    // start Wifi
    setupWiFi_STA();

    //start WebSocket server
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    Serial.print("Setup done!");
 }
/****************************************************************************************************/

void loop() {
    webSocket.loop();
    if (!hasClient) return; //for faster connect 
    
    //some trigger?? mechanisme with some timeout???
    for (int i=0;i<nrSamples;i++){
      int value_0 = analogRead(A0);
      delayMicroseconds(sampleTime);
      if ((value_0 > triggerLevel) && (value_0 < triggerLevel+10) && (value_0 < analogRead(A0))) break;
    }
    //Read 512 + 1 samples (waste 2 LS bits)
    for (int i=0;i<nrSamples;i++){
      trace[i] = (uint8_t) (analogRead(A0)>>2);
      delayMicroseconds(sampleTime);
    }
    webSocket.broadcastBIN(trace,513);//smaller canvas (511) would have been better
    delay(20);
}



