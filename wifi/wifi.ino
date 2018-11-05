#include"dht.h"                      // Including library for dht
#include<LiquidCrystal.h>
LiquidCrystal lcd(14,15,16,17,18,19);
#include<Timer.h>
Timer t;
#include <SoftwareSerial.h>
SoftwareSerial Serial1(2, 3);
#define dht_dpin 12 
#define heart 13
dht DHT;
char *api_key="K19KFJG73DW7N2VP";     // Enter your Write API key from ThingSpeak
static char postUrl[150];
int humi,tem;
void httpGet(String ip, String path, int port=80);
void setup()
{
 lcd.begin(16, 2);
 lcd.clear();
 lcd.print("   Humidity   ");
 lcd.setCursor(0,1);
 lcd.print("  Measurement ");
 delay(2000);
 lcd.clear();
 lcd.print("Circuit Digest ");
 lcd.setCursor(0,1);
 lcd.print("Welcomes You");
 delay(2000);
 Serial1.begin(9600);
 Serial.begin(9600);
 lcd.clear();
 lcd.print("WIFI Connecting");
 lcd.setCursor(0,1);
 lcd.print("Please wait....");
 Serial.println("Connecting Wifi....");
 connect_wifi("AT",1000);
 connect_wifi("AT+CWMODE=1",1000);
 connect_wifi("AT+CWQAP",1000);  
 connect_wifi("AT+RST",5000);
 connect_wifi("AT+CWJAP=\"Microcontrollerslab.com\",\"itulahore786\"",10000);
 Serial.println("Wifi Connected"); 
 lcd.clear();
 lcd.print("WIFI Connected.");
 pinMode(heart, OUTPUT);
 delay(2000);
 t.oscillate(heart, 1000, LOW);
 t.every(20000, send2server);
}
void loop()
{
  DHT.read11(dht_dpin);
  lcd.setCursor(0,0);
  lcd.print("Humidity: ");
  humi=DHT.humidity;
  lcd.print(humi);   // printing Humidity on LCD
  lcd.print(" %    ");
  lcd.setCursor(0,1);
  lcd.print("Temperature:");
  tem=DHT.temperature;
  lcd.print(tem);   // Printing temperature on LCD
  lcd.write(1);
  lcd.print("C   ");
  delay(1000);
  t.update();
}
void send2server()
{
  char tempStr[8];
  char humidStr[8];
  dtostrf(tem, 5, 3, tempStr);
  dtostrf(humi, 5, 3, humidStr);
  sprintf(postUrl, "update?api_key=%s&field1=%s&field2=%s",api_key,humidStr,tempStr);
  httpGet("api.thingspeak.com", postUrl, 80);
}
//GET https://api.thingspeak.com/update?api_key=K19KFJG73DW7N2VP&field1=0
void httpGet(String ip, String path, int port)
{
  int resp;
  String atHttpGetCmd = "GET /"+path+" HTTP/1.0\r\n\r\n";
  //AT+CIPSTART="TCP","192.168.20.200",80
  String atTcpPortConnectCmd = "AT+CIPSTART=\"TCP\",\""+ip+"\","+port+"";
  connect_wifi(atTcpPortConnectCmd,1000);
  int len = atHttpGetCmd.length();
  String atSendCmd = "AT+CIPSEND=";
  atSendCmd+=len;
  connect_wifi(atSendCmd,1000);
  connect_wifi(atHttpGetCmd,1000);
}
void connect_wifi(String cmd, int t)
{
  int temp=0,i=0;
  while(1)
  {
    lcd.clear();
    lcd.print(cmd);
    Serial.println(cmd);
    Serial1.println(cmd); 
    while(Serial1.available())
    {
      if(Serial1.find("OK"))
      i=8;
    }
    delay(t);
    if(i>5)
    break;
    i++;
  }
  if(i==8)
  {
   Serial.println("OK");
        lcd.setCursor(0,1);
      lcd.print("OK");
  }
  else
  {
   Serial.println("Error");
         lcd.setCursor(0,1);
      lcd.print("Error");
  }
}
