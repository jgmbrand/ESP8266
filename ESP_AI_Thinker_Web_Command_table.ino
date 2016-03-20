#include <ESP8266WiFi.h>

//////////////////////
// WiFi Definitions //
//////////////////////

//RED leds on AI_THINKER board
int gpios[9] = {2,0,4,5,14,16,15,12,13};

String stat[9] = {"LOW","LOW","LOW","LOW","LOW","LOW","LOW","LOW","LOW"};

WiFiServer server(80);

void setup() 
{
  initHardware();
  setupWiFi_STA();
  //setupWiFi_AP();
  server.begin();
}

//int val;
//String On_Off;

void loop() 
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) return;
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  //Serial.println(req.indexOf("/gpio2/0"));
  if (req.indexOf("/gpio2/0")   != -1){ digitalWrite(2, 0);   stat[0]="LOW";}  
  if (req.indexOf("/gpio2/1")   != -1){ digitalWrite(2, 1);   stat[0]="HIGH";} 
  if (req.indexOf("/gpio0/0")   != -1){ digitalWrite(0, 0);   stat[1]="LOW";}  
  if (req.indexOf("/gpio0/1")   != -1){ digitalWrite(0, 1);   stat[1]="HIGH";} 
  if (req.indexOf("/gpio4/0")   != -1){ digitalWrite(4, 0);   stat[2]="LOW";}  
  if (req.indexOf("/gpio4/1")   != -1){ digitalWrite(4, 1);   stat[2]="HIGH";} 
  if (req.indexOf("/gpio5/0")   != -1){ digitalWrite(5, 0);   stat[3]="LOW";}  
  if (req.indexOf("/gpio5/1")   != -1){ digitalWrite(5, 1);   stat[3]="HIGH";} 
  if (req.indexOf("/gpio14/0")  != -1){ digitalWrite(14, 0);  stat[4]="LOW";}  
  if (req.indexOf("/gpio14/1")  != -1){ digitalWrite(14, 1);  stat[4]="HIGH";} 
  if (req.indexOf("/gpio16/0")  != -1){ digitalWrite(16, 0);  stat[5]="LOW";}  
  if (req.indexOf("/gpio16/1")  != -1){ digitalWrite(16, 1);  stat[5]="HIGH";} 
 
  if (req.indexOf("/red/0")     != -1){ digitalWrite(15, 0);  stat[6]="LOW";}  
  if (req.indexOf("/red/1")     != -1){ digitalWrite(15, 1);  stat[6]="HIGH";} 
  if (req.indexOf("/green/0")   != -1){ digitalWrite(12, 0);  stat[7]="LOW";}  
  if (req.indexOf("/green/1")   != -1){ digitalWrite(12, 1);  stat[7]="HIGH";} 
  if (req.indexOf("/blue/0")    != -1){ digitalWrite(13, 0);  stat[8]="LOW";}  
  if (req.indexOf("/blue/1")    != -1){ digitalWrite(13, 1);  stat[8]="HIGH";} 

  client.flush();

  // Send the response to the client
  client.print(makepage());
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}
//**********************************************************
void setupWiFi_STA(){
  const char* ssid     = "linksys";
  const char* password = "0123456789";

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
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

//*****************************************************
void setupWiFi_AP()
{
  const char WiFiAPPSK[] = "sparkfun";
  WiFi.mode(WIFI_AP);

  // Do a little work to get a unique-ish name. Append the
  // last two bytes of the MAC (HEX'd) to "Thing-":
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = "ESP8266_" + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i=0; i<AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK,3);
  Serial.print("AP IP address: ");
  IPAddress myIP = WiFi.softAPIP();
  Serial.println(myIP);
}

//*****************************************************
void initHardware()
{
  delay(1000);
  Serial.begin(115200);
  Serial.println();Serial.println();
  Serial.println(__FILE__);Serial.println(__DATE__);

  for (int i=0;i<9;i++){ pinMode(gpios[i], OUTPUT);digitalWrite(gpios[i], 1);}
}

String makepage(void){
    // Prepare the response. Start with the common header:
    String s = "HTTP/1.1 200 OK Content-Type: text/html\r\n\r\n"\
    "<!DOCTYPE HTML>"\
    "<html>";
    s +="<body bgcolor = LightGray>";
    s +="<h3 style= color:blue >Click buttons to set GPIO levels</h3>";
    s +="<style>";
    s +="table,th,td{border:2px solid black;border-collapse:collapse;width:180px;}";
    s +="td{text-align:center;}";
    s +="</style>";
    s +="<table>";
    s +="  <tr>";
    s +="  <td style=text-align:left;>GPIO2:</td>";
    s +=" <td style=text-align:center;width:60px;>";
    s +="   <button onclick= window.location.href='/gpio2/0' >0</button>";
    s +="   <button onclick= window.location.href='/gpio2/1' >1</button>";
    s +=" </td>";
    s +=" <td style= color:white;background-color:black;>"+stat[0]+"</td>";
    s +="  </tr>";
    s +="  <tr>";
    s +=" <td style=text-align:left;>GPIO0:</td>";
    s +=" <td style=text-align:center;>";
    s +="   <button onclick= window.location.href='/gpio0/0' >0</button>";
    s +="   <button onclick= window.location.href='/gpio0/1' >1</button>";
    s +=" </td>";
    s +=" <td style= color:white;background-color:black;>"+stat[1]+"</td>";
    s +="  </tr>";
    s +="  <tr>";
    s +=" <td style=text-align:left;>GPIO4:</td>";
    s +=" <td style=text-align:center;>";
    s +="   <button onclick= window.location.href='/gpio4/0' >0</button>";
    s +="   <button onclick= window.location.href='/gpio4/1' >1</button>";
    s +=" </td>";
    s +=" <td style= color:white;background-color:black;>"+stat[2]+"</td>";
    s +="  </tr>";
    s +="  <tr>";
    s +=" <td style=text-align:left;>GPIO5:</td>";
    s +=" <td style=text-align:center;>";
    s +="   <button onclick= window.location.href='/gpio5/0' >0</button>";
    s +="   <button onclick= window.location.href='/gpio5/1' >1</button>";
    s +=" </td>";
    s +=" <td style= color:white;background-color:black;>"+stat[3]+"</td>";
    s +="  </tr>";
    s +="  <tr>";
    s +=" <td style=text-align:left;>GPIO14:</td>";
    s +=" <td style=text-align:center;>";
    s +="   <button onclick= window.location.href='/gpio14/0' >0</button>";
    s +="   <button onclick= window.location.href='/gpio14/1' >1</button>";
    s +=" </td>";
    s +=" <td style= color:white;background-color:black;>"+stat[4]+"</td>";
    s +="  </tr>";
    s +="  <tr>";
    s +=" <td style=text-align:left;>GPIO16:</td>";
    s +=" <td style=text-align:center;>";
    s +="   <button onclick= window.location.href='/gpio16/0' >0</button>";
    s +="   <button onclick= window.location.href='/gpio16/1' >1</button>";
    s +=" </td>";
    s +=" <td style= color:white;background-color:black;>"+stat[5]+"</td>";
    s +="  </tr>";
    s +="  <tr>";
    s +=" <td style=text-align:left;>GPIO15:</td>";
    s +=" <td style=text-align:center;>";
    s +="   <button onclick= window.location.href='/red/0' >0</button>";
    s +="   <button onclick= window.location.href='/red/1' >1</button>";
    s +=" </td>";
    s +=" <td style= color:white;background-color:black;>"+stat[6]+"</td>";
    s +="  </tr>";
    s +="  <tr>";
    s +=" <td style=text-align:left;>GPIO12:</td>";
    s +=" <td style=text-align:center;>";
    s +="   <button onclick= window.location.href='/green/0' >0</button>";
    s +="   <button onclick= window.location.href='/green/1' >1</button>";
    s +=" </td>";
    s +=" <td style= color:white;background-color:black;>"+stat[7]+"</td>";
    s +="  </tr>";
    s +="  <tr>";
    s +=" <td style=text-align:left;>GPIO13:</td>";
    s +=" <td style=text-align:center;>";
    s +="   <button onclick= window.location.href='/blue/0' >0</button>";
    s +="   <button onclick= window.location.href='/blue/1' >1</button>";
    s +=" </td>";
    s +=" <td style= color:white;background-color:black;>"+stat[8]+"</td>";
    s +="  </tr>";
    s +="</table>";
    s +="</body>";
    s +="</html>";

    return (s);
}

//if (val >= 0)
  //{
  //  s += "LED is now ";
  //  s += (val)?"off":"on";
  //}
  //else if (val == -2)
  //{ // If we're reading pins, print out those values:
  //  s += "Analog Pin = ";
  //  s += String(analogRead(ANALOG_PIN));
  //  s += "<br>"; // Go to the next line.
  //  s += "Digital Pin 12 = ";
  //  s += String(digitalRead(DIGITAL_PIN));
  //}
  //else
  //{
  //  s += "Invalid Request.<br> Try /led/1, /led/0, or /read.";
  //}
  //  s += "<p id=\"demo\"></p>";
  //  s += "<script>";
  //  s += "document.getElementById(\"demo\").innerHTML ="; 
  //  s += "\"Page location is: \" + window.location.href";
  //  s += "</script>";
 
  
