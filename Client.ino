#include <SPI.h>
#include <WiFi.h>
#include <DHT11.h>


//////////////////////////////pH/////////////////////////////////
#define SensorPin A0            
#define Offset 0.00            
#define LED 13
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    
int pHArray[ArrayLenth];  
int pHArrayIndex=0;    
/////////////////////////////////////////////////////////////////

//////////////////////////////조도//////////////////////////////
int light = A1;
int lightValue = 0;
///////////////////////////////////////////////////////////////

//////////////////////////온습도///////////////////////////////
int pin=2;    // 연결한 아두이노 디지털 핀 번호
DHT11 dht11(pin);
///////////////////////////////////////////////////////////////

int status = WL_IDLE_STATUS;
char ssid[] = "ollehEgg_455"; //  your network SSID (name) 
char pass[] = "info86055"; 

IPAddress remoteIp(192,168,1,10);
int port = 65505;

String message = "";

WiFiClient client;


void setup()
{
    // start the serial for debugging
    pinMode(LED,OUTPUT); 
    Serial.begin(9600);

  
    if(WiFi.status() == WL_NO_SHIELD){
        Serial.println("WiFi shield not present! Press reset to try again.");
        while(true); //stops the program
    }

    connectWiFi();
    printWifiStatus();
    connectClient(3);
}

void loop(){  
    if(client){
      Serial.println("receivemessage");
        if(client.available()){
            Serial.println("receivemessage2");
            char c = client.read();
            while (c != '\n') {
                message +=c;
                c = client.read();
            }
           Serial.println(message);
             checkMessage(message);
             message = "";
             

        }
    }

 
 /////////////////////////pH//////////////////////////////
    static unsigned long samplingTime = millis();
    static unsigned long printTime = millis();
    static float pHValue,voltage;
    if(millis()-samplingTime > samplingInterval)
    {
        pHArray[pHArrayIndex++]=analogRead(SensorPin);
        if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
        voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
        pHValue = 3.5*voltage+Offset;
        samplingTime=millis();
    }
    if(millis() - printTime > printInterval)

    {
        Serial.print(pHValue,2);
        digitalWrite(LED,digitalRead(LED)^1);
        printTime=millis();
    }
 ////////////////////////////////////////////////////////// 

////////////////////////조도///////////////////////////////
    lightValue = analogRead(light);
///////////////////////////////////////////////////////////

///////////온습도/////////////////////////////////////////
    int err;
    float temp, humi;
    if((err=dht11.read(humi, temp))==0)
    {
      Serial.print(temp);
      Serial.print("\n");
      Serial.print(humi);
    }
    else
    {
      Serial.println();
      Serial.print("Error No :");
      Serial.print(err);
      Serial.println();    
     }
      delay(10000); //delay for reread
//////////////////////////////////////////////////////////

////////////////////////////Server전송////////////////////////
    if( status == WL_CONNECTED){
      client.print(temp);
      delay(5000);
      client.print(humi);
      delay(5000);
      client.print(lightValue);
      delay(5000);
      client.print(pHValue,2);
     // delay(3000);
      //client.print(ECcurrent,2);
      delay(20000);
    }
//////////////////////////////////////////////////////////////////    
}

////////////////////////////pH function///////////////////////////////
double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}
/////////////////////////////////////////////////////////////////////

void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
}

void connectWiFi(){

    if( status != WL_CONNECTED){
        while(status != WL_CONNECTED){

            Serial.print("Attempting connection to network...");

            status = WiFi.begin(ssid, pass);
            delay(3000);

            if(status == WL_CONNECTED){
                Serial.println(" SUCSESS");
            }
            else{
                Serial.println(" FAILED");
                delay(3000);
                connectWiFi();
            }
        }
    }   
}

void connectClient(int retries){

    //Attempt connection to server

    if(retries <= 0){
        Serial.println("FAILED");
        Serial.println("Connection to server failed.");
        while(true);
    }

    Serial.print("Attempting conenction to server... ");

    if(client.connect(remoteIp, port)){
        Serial.println("SUCSESS");         
        //sendMessage("Hello");
    }
    else if(retries > 0){
        Serial.println("FAILED");
        connectClient(retries - 1);
    }

}

void checkMessage(String message){

    if(message=="airOn"){
        digitalWrite(9, HIGH);
        sendMessage("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    }
    else if(message == "airOff"){
        digitalWrite(9, LOW);
        sendMessage("cccccccccccccccccccccccccccc");
    }
  //  else if (message == "ledOn"){
 //   }
 //   else if (message == "ledOff"){
  //  }
     else{
    
      Serial.println("fail");
      sendMessage("fail");
    }
}

void sendMessage(String toSend){

    if(client){
        client.println(toSend+'\n');
        client.flush();
        Serial.println("Sendt message: "+toSend);
    }
    else{
        Serial.println("Could not send message; Not connected.");
    }
    delay(1000);
}


