/*
 * Arduino code for controlling a DC motor's speed and direction with an ESP8266 using a browser on a device in local network.
 * 
 * Made by KushagraK7: https://www.instructables.com/member/KushagraK7/
 * 
 * The ESP8266 and the controlling device such as the laptop or the smartphone should be connected to the same local network.
 * 
 * 
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "PageIndex.h"; //--> Include the contents of the User Interface Web page, stored in the same folder as the .ino file

#define LEDonBoard 12  //--> Defining an On Board LED, used for indicators when the process of connecting to a wifi router

#define OutA 0
#define OutB 2

#define YES 1
#define NO 0

//----------------------------------------SSID and Password of your WiFi router
const char *ssid = "Your_Wi-Fi_Router's_name";
const char *password = "Router_Password";
//----------------------------------------

int pos;
int p = 0;
int m = 1;
int sp;
float spe = 0;

int acc = YES;//If the value is 'YES', the motor will accelerate, if it is 'NO', it will not.

String POS;
String Dir;

ESP8266WebServer server(80);  //--> Server on port 80

//----------------------------------------This routine is executed when you open NodeMCU ESP8266 IP Address in browser
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
//----------------------------------------

//----------------------------------------Procedure for handling servo control
void setSpeed()
{
  POS = server.arg("POS");
  
  p = POS.toInt(); 
  
  Serial.print("Position:  ");
  Serial.print(p);
  Serial.print("  Direction:  ");
  Serial.println(m);
  server.send(200, "text/plane","");

  
}

void setDir()
{
  Dir = server.arg("Dir");

  if(Dir.length()  != 0) m = Dir.toInt();

  /*
  Serial.print("Position:  ");
  Serial.print(p);
  Serial.print("  Direction:  ");
  Serial.println(m);
  server.send(200, "text/plane","");
  */
}

void run(int s)
{
  if(s > 0 && s <= 255)
  {
    digitalWrite(OutB, LOW);
    analogWrite(OutA, s);
  }

  if(s < 0 && s >= -255)
  {
    digitalWrite(OutA, LOW);
    analogWrite(OutB, -s);
  }

  else
  {
    digitalWrite(OutB, LOW);
    digitalWrite(OutA, LOW);
  }

}
//----------------------------------------
//----------------------------------------Setup----------------------------------------
void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(OutA, OUTPUT);
  pinMode(OutB, OUTPUT);
  
  
  WiFi.begin(ssid, password); //--> Connect to your WiFi router
  Serial.println("");
    
  pinMode(LEDonBoard,OUTPUT); //--> On Board LED port Direction output
  digitalWrite(LEDonBoard, HIGH); //--> Turn off Led On Board
  
  //----------------------------------------Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    //----------------------------------------Make the On Board Flashing LED on the process of connecting to the wifi router.
    digitalWrite(LEDonBoard, LOW);
    delay(250);
    digitalWrite(LEDonBoard, HIGH);
    delay(250);
    //----------------------------------------
  }
  //----------------------------------------
  digitalWrite(LEDonBoard, HIGH); //--> Turn off the On Board LED when it is connected to the wifi router.
  //----------------------------------------If connection is successful then show IP address in serial monitor
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //----------------------------------------

  //----------------------------------------Initialize Webserver
  server.on("/",handleRoot);  //--> Routine to handle at root location. This is to display web page.
  server.on("/setPOS",setSpeed); //--> Sets servo position from Web request
  server.on("/setDir",setDir);
  server.begin();  
  Serial.println("HTTP server started");
}
//------------------------------------------------------------------------------------
//----------------------------------------Loop----------------------------------------
void loop() {
 server.handleClient();

 sp = map(p, 0, 100, 0, 255);

 if(spe < sp*m) spe+= 0.1;
 if(spe > sp*m) spe-= 0.1;

 if(acc == 1) run(spe);
 else run(sp*m);
}
//------------------------------------------------------------------------------------
