#include <SoftwareSerial.h>
#define RX 4
#define TX 3
#define sensorPower 7
#define sensorPin A0
#define relay 10

String AP = "YOUR ESSID";       // AP NAME
String PASS = "Your Password"; // AP PASSWORD
String API = "YOUR API KEY";   // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
String field = "field1";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;
SoftwareSerial esp8266(RX,TX);
 
  
void setup() {
  Serial.begin(9600);
  esp8266.begin(115200);
  pinMode(sensorPower, OUTPUT);
 // pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);
  // Initially keep the sensor OFF
  digitalWrite(sensorPower, LOW);
  digitalWrite(relay, HIGH);  
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
}

int readSensor() 
{  
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  
  //delay(1000); // Allow power to settle
  int val = analogRead(sensorPin);  // Read the analog value form sensor
  if(val<750)
  {digitalWrite(relay,HIGH);
  }
  if(val>750)
  {
  digitalWrite(relay,LOW);
  }
    
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return val;             // Return analog moisture value
}

void loop() 
{
  Serial.print("Analog output: ");
  Serial.println(readSensor());
  //delay(1000);
  
 valSensor = readSensor();
 String getData = "GET /update?api_key="+ API +"&"+ field +"="+String(valSensor);
sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);
 delay(1500);
 countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
}

int getSensorData(){
  
  return random(1000); // Replace with your own sensor code
}

void sendCommand(String command, int maxTime, char readReplay[]) 
{
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
