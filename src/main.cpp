#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define output12v2 D4
#define output12v1 D3
#define relay2 D2
#define relay1 D1

#define MQTT_AUTH true
#define MQTT_USERNAME "<MQTT_USERNAME>"
#define MQTT_PASSWORD "<MQTT_PASSWORD>"

const char* ssid = "<SSID>";
const char* password = "<SSID_PASSWORD>";

const String HOSTNAME  = "<HOSTNAME>";
const String MQTT_LIGHT_TOPIC1 = "<TOPIC1>";
const String MQTT_LIGHT_TOPIC2 = "<TOPIC2>";
const String MQTT_LIGHT_TOPIC3 = "<TOPIC3>";
const String MQTT_LIGHT_TOPIC4 = "<TOPIC4>";
const String MQTT_LIGHT_STATE_TOPIC1 = "<STATUS_TOPIC1>";
const String MQTT_LIGHT_STATE_TOPIC2 = "<STATUS_TOPIC2>";
const String MQTT_LIGHT_STATE_TOPIC3 = "<STATUS_TOPIC3>";
const String MQTT_LIGHT_STATE_TOPIC4 = "<STATUS_TOPIC4>";
const char* WILL_TOPIC = "devices/will";
const String DEVICE_TOPIC = "devices/list";
const char* WILL_MESSAGE = "<WILL_MESSAGE>";
const char* MQTT_SERVER = "<MQTT_SERVER";

WiFiClient wclient;
PubSubClient client(MQTT_SERVER,1883,wclient);

void turn_on(int billy){  
  if( billy == 1 ){
    digitalWrite(output12v1,HIGH);
    client.publish(MQTT_LIGHT_STATE_TOPIC1.c_str(),"ON",true);
    Serial.println("on");
  }
  if (billy == 2 ) {
    digitalWrite(output12v2,HIGH);
    client.publish(MQTT_LIGHT_STATE_TOPIC2.c_str(),"ON",true);
  } 
  if (billy == 3 ) {
    digitalWrite(relay1,HIGH);
    client.publish(MQTT_LIGHT_STATE_TOPIC3.c_str(),"ON",true);
  } 
  if (billy == 4 ) {
    digitalWrite(relay2,HIGH);
    client.publish(MQTT_LIGHT_STATE_TOPIC4.c_str(),"ON",true);
  }     
}
//-------------------------------------------

void turn_off(int billy){
  if( billy == 1 ){
    digitalWrite(output12v1,LOW);
    client.publish(MQTT_LIGHT_STATE_TOPIC1.c_str(),"OFF",true);
    Serial.println("off");
  }
  if (billy == 2 ){
    digitalWrite(output12v2,LOW);
    client.publish(MQTT_LIGHT_STATE_TOPIC2.c_str(),"OFF",true);  
  }  
  if (billy == 3 ){
    digitalWrite(relay1,LOW);
    client.publish(MQTT_LIGHT_STATE_TOPIC3.c_str(),"OFF",true);  
  }  
  if (billy == 4 ){
    digitalWrite(relay2,LOW);
    client.publish(MQTT_LIGHT_STATE_TOPIC4.c_str(),"OFF",true);  
  }      
}
//-------------------------------------------
void callback(char* topic, byte* payload, unsigned int length) {
  String payloadStr = "";
  Serial.println("callback");
  for (int i=0; i<length; i++) {
    payloadStr += (char)payload[i];
  }
  Serial.println(payloadStr);
  String topicStr = String(topic);
  if(topicStr.equals(MQTT_LIGHT_TOPIC1)){
    if(payloadStr.equals("off")){
      turn_off(1);
    }
    else if(payloadStr.equals("on")){
      turn_on(1);
    }     
  } 
  if(topicStr.equals(MQTT_LIGHT_TOPIC2)){
    if(payloadStr.equals("off")){
      turn_off(2);
    }
    else if(payloadStr.equals("on")){
      turn_on(2);
    }     
  }   
  if(topicStr.equals(MQTT_LIGHT_TOPIC3)){
    if(payloadStr.equals("off")){
      turn_off(3);
    }
    else if(payloadStr.equals("on")){
      turn_on(3);
    }     
  }   
  if(topicStr.equals(MQTT_LIGHT_TOPIC4)){
    if(payloadStr.equals("off")){
      turn_off(4);
    }
    else if(payloadStr.equals("on")){
      turn_on(4);
    }     
  }       
}
//-------------------------------------------
bool checkMqttConnection(){
  if (!client.connected()) {
    if (client.connect(HOSTNAME.c_str(),MQTT_USERNAME, MQTT_PASSWORD, WILL_TOPIC, 0, 0, WILL_MESSAGE, 0)) {
      Serial.println("CONNECTED");
      client.subscribe(MQTT_LIGHT_TOPIC1.c_str());
      client.subscribe(MQTT_LIGHT_TOPIC2.c_str());
      client.subscribe(MQTT_LIGHT_TOPIC3.c_str());
      client.subscribe(MQTT_LIGHT_TOPIC4.c_str());            
      client.publish(DEVICE_TOPIC.c_str(),HOSTNAME.c_str(),true);
    }
  }
  return client.connected();
}
//-------------------------------------------
void connectWifi(){
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA);
  Serial.println(".");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Ligado a ");
  Serial.println(ssid);
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}
//-------------------------------------------
void setup() {
  Serial.begin(115200);
  client.setCallback(callback);  
  pinMode(output12v1,OUTPUT);
  pinMode(output12v2,OUTPUT);
  pinMode(relay1,OUTPUT);
  pinMode(relay2,OUTPUT);
  connectWifi();


}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (checkMqttConnection()){
      client.loop();
    }
  }else{
    connectWifi();
  }
}