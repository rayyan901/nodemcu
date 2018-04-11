

#include "ThingSpeak.h"                                 // Include ThingSpeak Library
#include <ESP8266WiFi.h>                                // Include ESP8266wifi Library
#include <ESP8266WiFiMulti.h>
//====================
//#include <SimpleTimer.h>
#include <SoftwareSerial.h>

#define rxPin D1
#define txPin D2
SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);
ESP8266WiFiMulti wifiMulti;

////total string lenght. The first message is 25 characters long (including spaces)
int dataLength=25;
////
char incomingByte; //incoming data from the BT link
////
////
////// the message array
char Phrase[30];
char filtData[30];

float fData=0;
//String filtData = "";
//

//========  
//char ssid[] = "SEE_WIFI_2A";                               // Enter your WiFi SSID (router name)
//char pass[] = "ppkeewifi";                             // Enter your WiFi router password
unsigned long myChannelNumber = 471867;                 // Enter your Thingspeak Channel Number
const char * myWriteAPIKey = "UY78Q8L5ZQVJETXD";        // Enter your ThingSpeak API Key


unsigned long currentMillis = millis();
unsigned long previousMillis = 0;
unsigned long resetMillis = 0;
const long interval = 90000;                            // Time delay interval (90 seconds) to prevent false triggers when setting up device 
                                                        
int status = WL_IDLE_STATUS;
WiFiClient  client;

void setup() 
{ 
   pinMode(LED_BUILTIN, OUTPUT);
   // define pin modes for tx, rx:
 digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
   delay(500);

   
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  Serial.begin(115200);
//  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
//====
  wifiMulti.addAP("SEE_WIFI_2A", "ppkeewifi");            // Listado de SSIDs
  wifiMulti.addAP("SEE_WIFI_2B", "ppkeewifi");
  wifiMulti.addAP("j7pro", "abcd1234");
//===   
while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(1000);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("SSID:\t\t");
  Serial.println(WiFi.SSID());              // SSID al que se ha conectado.
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // IP del ESP8266.
 
  ThingSpeak.begin(client);
 //==========
//  WiFi.begin(ssid, pass);                               // Start WiFi connection
//  ThingSpeak.begin(client);                             // Start ThingSpeak connection
                                   // Serial Baud Rate
//  
//  Serial.println();
//  Serial.print("Connecting to ");
//  Serial.print(ssid); 
//  while (WiFi.status() != WL_CONNECTED) 
//  {
//    delay(500);
//    Serial.print(".");
//  }
  Serial.println();
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
  Serial.println("LandSlide Detector");

 
 
}

void loop() 
{
//********************* Read motion sensor and upload to ThingSpeak ***************************** 
  
currentMillis = millis();                                                 // Set the currentMillis equal to the millis function


//===========


  //or, if we get something on the virtual bluetooth port...
    if (mySerial.available() > 0) {

        dataLength = mySerial.available(); //incoming string lenght

        for  (int i = 0; (i < dataLength) && (i<30); i++) {

            // read the incoming byte:
            incomingByte = mySerial.read();

            Phrase[i] = incomingByte; ///store the character into the string array

        }
        //=====
        int j=0;
        int i=0;
        while(Phrase[i]!='S' && i<30) {
        i++;
        }
        i++;
        while(Phrase[i]!= '\r' && i<30) {
        filtData[j]=Phrase[i];
        i++;
        j++;
        }
          
        
        fData=atof(filtData); //.toFloat();
        
        //debug features, to check :
        Serial.println("Display: ");
        Serial.println(Phrase);
        Serial.print("Length : ");
        Serial.println(dataLength);
        Serial.println("Filtered : ");
        Serial.println(filtData);
    }
 //========  
if(fData>30.0) {
ThingSpeak.writeField(myChannelNumber, 1, fData, myWriteAPIKey);
Serial.print("Data : ");// Sent to ThingSpeak!");
Serial.print(fData);
Serial.println("  Sent to ThingSpeak!");
 digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW

}

//====================


   
 ///////////////////////////////////
 if (currentMillis - previousMillis >= 30000)                             // Update Serial Monitor every ten seconds (10000 milliseonds)
 {                                                                        // as to how long the program has been running
 Serial.print("Program running for ");
 Serial.print((currentMillis/1000)/60);
 Serial.println(" Minutes"); 
 previousMillis = currentMillis;
 
 //================================================// ThingSpeak will only accept updates every 15 seconds.
ThingSpeak.writeField(myChannelNumber, 1, fData, myWriteAPIKey);
Serial.print("Data : ");// Sent to ThingSpeak!");
Serial.print(fData);
Serial.println("  Sent to ThingSpeak!");
//========================================================== 
 
 yield();                                                                 // Yield to prevent the WDT from crashing the NodeMCU
 digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW

  //delay(100);              
 
 }
delay(1000); 
 
}  

