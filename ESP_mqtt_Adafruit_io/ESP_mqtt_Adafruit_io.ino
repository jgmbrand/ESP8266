
/**************************************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************************************/

//ATTENTION:
// csv payload data needed more then 20 characters.
// therefore in file  "Adafruit_MQTT.h" changed payload size:
//#define SUBSCRIPTIONDATALEN 20 to  #define SUBSCRIPTIONDATALEN 60
// to use the subscription feeds gpio0 and gpio2, MLX90615 calls must be disabled. 
// MLX90615 calls are marked /*########################*/

/****************************************************************************/
#include "credentials.h"

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/******* Temperature sensor **************************************************/
#include <I2C_BB.h>
#include "MLX90615.h"

/************************* WiFi Access Point *********************************/
#define _SSID YOUR_SSID
#define _PASS YOUR_PASS

/************************* Adafruit.io Setup *********************************/
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                //8883 for SSL
#define AIO_USERNAME    YOUR_AIO_USERNAME   //define in credentials.h
#define AIO_KEY         YOUR_AIO_KEY        //define in credentials.h

#define GPS_LOCATION1   YOUR_GPS_LOCATION1  //define in credentials.h
#define GPS_LOCATION2   YOUR_GPS_LOCATION2  //define in credentials.h

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class(object) to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Store the MQTT server, username, and password in flash memory.
// This is required for using the Adafruit MQTT library.
const char MQTT_SERVER[] PROGMEM    = AIO_SERVER;
const char MQTT_USERNAME[] PROGMEM  = AIO_USERNAME;
const char MQTT_PASSWORD[] PROGMEM  = AIO_KEY;

const char GPS_LOC1[] PROGMEM  = GPS_LOCATION1;
const char GPS_LOC2[] PROGMEM  = GPS_LOCATION2;

bool locationSelect;
void MQTT_connect(void);

//Setup the MQTT client class(object) by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, AIO_SERVERPORT, MQTT_USERNAME, MQTT_PASSWORD);

/****************************** Feeds ***************************************/
//Notice : MQTT paths for AIO follow the form: <username>/feeds/<feedname>

const char PHOTOCELL_FEED[] PROGMEM = AIO_USERNAME "/feeds/l_ambient/csv";
Adafruit_MQTT_Publish l_ambient = Adafruit_MQTT_Publish(&mqtt, PHOTOCELL_FEED);

const char AMBIENT_FEED[] PROGMEM = AIO_USERNAME "/feeds/t_ambient";
Adafruit_MQTT_Publish t_ambient = Adafruit_MQTT_Publish(&mqtt, AMBIENT_FEED);

const char OBJECT_FEED[] PROGMEM = AIO_USERNAME "/feeds/t_object";
Adafruit_MQTT_Publish t_object = Adafruit_MQTT_Publish(&mqtt, OBJECT_FEED);

/**************************** subsciptions ************************************/
//Notice : MQTT paths (topics) for AIO follow the form: <username>/feeds/<feedname>

const char GPIO2_FEED[] PROGMEM = AIO_USERNAME "/feeds/GPIO2";
Adafruit_MQTT_Subscribe gpio2 = Adafruit_MQTT_Subscribe(&mqtt, GPIO2_FEED);

const char GPIO0_FEED[] PROGMEM = AIO_USERNAME "/feeds/GPIO0";
Adafruit_MQTT_Subscribe gpio0 = Adafruit_MQTT_Subscribe(&mqtt, GPIO0_FEED);

const char GPIO4_FEED[] PROGMEM = AIO_USERNAME "/feeds/GPIO4";
Adafruit_MQTT_Subscribe gpio4 = Adafruit_MQTT_Subscribe(&mqtt, GPIO4_FEED);

const char GPIO5_FEED[] PROGMEM = AIO_USERNAME "/feeds/GPIO5";
Adafruit_MQTT_Subscribe gpio5 = Adafruit_MQTT_Subscribe(&mqtt, GPIO5_FEED);

const char GPIO14_FEED[] PROGMEM = AIO_USERNAME "/feeds/GPIO14";
Adafruit_MQTT_Subscribe gpio14 = Adafruit_MQTT_Subscribe(&mqtt, GPIO14_FEED);

const char GPIO16_FEED[] PROGMEM = AIO_USERNAME "/feeds/GPIO16";
Adafruit_MQTT_Subscribe gpio16 = Adafruit_MQTT_Subscribe(&mqtt, GPIO16_FEED);

const char GPIO15_FEED[] PROGMEM = AIO_USERNAME "/feeds/GPIO15";
Adafruit_MQTT_Subscribe gpio15 = Adafruit_MQTT_Subscribe(&mqtt, GPIO15_FEED);

const char GPIO12_FEED[] PROGMEM = AIO_USERNAME "/feeds/GPIO12";
Adafruit_MQTT_Subscribe gpio12 = Adafruit_MQTT_Subscribe(&mqtt, GPIO12_FEED);

const char GPIO13_FEED[] PROGMEM = AIO_USERNAME "/feeds/GPIO13";
Adafruit_MQTT_Subscribe gpio13 = Adafruit_MQTT_Subscribe(&mqtt, GPIO13_FEED);

/********************* Sketch defines ********************************/
#define GPIO0 0
#define GPIO2 2
#define GPIO4 4
#define GPIO5 5
#define GPIO14 14
#define GPIO16 16
#define GPIO15 15
#define GPIO12 12
#define GPIO13 13

/*********************** Sketch setups *******************************/

/*************************** GPIO ************************************/
void setupGPIO(void){
  pinMode(GPIO0, OUTPUT); 
  pinMode(GPIO2, OUTPUT); 
  pinMode(GPIO4, OUTPUT); 
  pinMode(GPIO5, OUTPUT); 
  pinMode(GPIO14, OUTPUT); 
  pinMode(GPIO16, OUTPUT); 
  pinMode(GPIO15, OUTPUT); 
  pinMode(GPIO12, OUTPUT); 
  pinMode(GPIO13, OUTPUT); 
}
//*****************************************************
void setupWIFI_AP(void){
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP8266_MQTT");
  delay(1000);
}
/*************************** WIFI ************************************/
void setupWIFI_STA(void){
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(_SSID);
  WiFi.begin(_SSID, _PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
}

/*************************** MQTT ************************************/
void setupMQTT(void){
  //Setup MQTT subscriptions.
  mqtt.subscribe(&gpio0);
  mqtt.subscribe(&gpio2);
  mqtt.subscribe(&gpio4);
  mqtt.subscribe(&gpio5);
  mqtt.subscribe(&gpio14);
  mqtt.subscribe(&gpio16);
  mqtt.subscribe(&gpio15);
  mqtt.subscribe(&gpio12);
  mqtt.subscribe(&gpio13);
}

boolean firstTime;

/************************** Setup ************************************/
void setup() {
  Serial.begin(115200);
  Serial.println();Serial.println();
  Serial.println(__FILE__);
  Serial.println(F("Adafruit MQTT demo"));
  setupGPIO();
  setupWIFI_STA();
  setupMQTT();
  firstTime = true;
}

/************************** Loop ************************************/
void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(3000))) {
    Serial.println();
    Serial.println("###################################################################");
    Serial.println();
    
    Serial.print(F("Got from "));Serial.print(String(subscription->topic));
    Serial.print(F(": "));Serial.println((char *)(subscription->lastread));

    //dasboard switches must send only : '0' or '1'
    int gpioStatus = subscription->lastread[0] - 0x30;                   
    
    if (subscription == &gpio2) digitalWrite(GPIO2, gpioStatus);
    if (subscription == &gpio0) digitalWrite(GPIO0, gpioStatus);
    if (subscription == &gpio4) digitalWrite(GPIO4, gpioStatus);
    if (subscription == &gpio5) digitalWrite(GPIO5, gpioStatus);
    if (subscription == &gpio14) digitalWrite(GPIO14, gpioStatus);
    if (subscription == &gpio16) digitalWrite(GPIO16, gpioStatus);
    if (subscription == &gpio15) digitalWrite(GPIO15, gpioStatus);
    if (subscription == &gpio12) digitalWrite(GPIO12, gpioStatus);
    if (subscription == &gpio13) digitalWrite(GPIO13, gpioStatus);
  }
  // Now we can publish stuff!
  
  // Send photocell value together with its location data as csv data
  // Using String-type may be the simplest methode to create the csv data 
  //String sendBuffer = String(analogRead(A0)) + ", 52.3793956, 6.6714914, 10.5432";
  String sendBuffer = String(analogRead(A0));
  if (locationSelect){
    locationSelect = false;
    sendBuffer += String(GPS_LOC1);
  }else{
    locationSelect = true;
    sendBuffer += String(GPS_LOC2);
  }
  
    
  //sendBuffer = String(analogRead(A0)) + String(GPS_LOC2);
  Serial.print(F("\nSending photocell value ")); Serial.print(sendBuffer);
  Serial.print("...");
  l_ambient.publish((char *)sendBuffer.c_str())?Serial.println(F("OK")):Serial.println(F("FAIL!"));

  //Send ambient temperature value
  /*########################*/
  float temp;
  //temp = mlx90615_Temperature(AMBIENT_TEMPERATURE);
  Serial.print(F("\nSending ambient temperature value ")); Serial.print(temp);
  Serial.print("...");
  t_ambient.publish(temp)?Serial.println(F("OK")):Serial.println(F("FAIL!"));
  
  //Send object temperature value
  /*########################*/
  //temp = mlx90615_Temperature(OBJECT_TEMPERATURE);
  Serial.print(F("\nSending object temperature value ")); Serial.print(temp);
  Serial.print("...");
  t_object.publish(temp)?Serial.println(F("OK")):Serial.println(F("FAIL!"));
}

/***********************************************************************************/
//Function to connect and reconnect as necessary to the MQTT server.
//Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Exit if already connected.
  if (mqtt.connected()) return;

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) while (1); // basically die and wait for WDT to reset me
  }
  Serial.println("MQTT Connected!");
}
  

