
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <time.h>

WiFiUDP ntpUDP;
//WiFi info 
const char* ssid = "GGUEGGUE";
const char* password = "11118888";
//const char* ssid = "PHONGKHACHDHBL";
//const char* password = "dhbl2006";
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
String humi;
String times;
//릴레이 핀
int relay = D10;

//GMT +7 = 25200
const long utcOffsetInSeconds = 25200;
NTPClient timeClient(ntpUDP , "pool.ntp.org" , utcOffsetInSeconds);

 void SensingLCD(int moisture){
  Serial.print("Moisture Sensor Value:");
  Serial.println(moisture);
  delay(2000);
} 

  String TimeChecker(){
   timeClient.update();
   const char* day = daysOfTheWeek[timeClient.getDay()];
   String hour =(String) timeClient.getHours() + ":";
   String minutes =(String) timeClient.getMinutes()+ ":";
   String seconds =  (String)timeClient.getSeconds();
  
   String times = hour + minutes + seconds;

   return times;
  }
//HTTP 통신 
  void NetWorkTask(String humi, String times){
    if (WiFi.status() == WL_CONNECTED) { 
       HTTPClient http;  
       http.begin("http://192.168.43.136:3000/smart_humid?humid="+humi+"&humid_date="+times+"");
       int httpCode = http.GET();                                                                 
      
      if (httpCode == HTTP_CODE_OK) { 
        
        Serial.print("HTTP response code ");
        Serial.println(httpCode); 
        String response = http.getString();   
        Serial.println(response);      
      }else{
        Serial.println("DO Not Request!!!");
      }
    http.end();   //Close connection
  }
}
  
void setup () {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
 pinMode (relay, OUTPUT); 
 
while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
}
  Serial.println("WiFi Connect!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  timeClient.begin();
}

void loop() {
  int hi = analogRead(0);
  int moisture =(map(analogRead(0), 890 ,360 ,0,100));
  humi = (String)moisture;
  Serial.println("습도 수치 :" + hi);
  Serial.println("형변환 습도 :" + humi + "%");

     if(moisture < 40) { 
          digitalWrite(relay, HIGH); //릴레이 on
          Serial.println("Relay ON!!!");
        } else if(moisture> 60) { 
          digitalWrite(relay, LOW); //릴레이 off
           Serial.println("Relay ON!!!");
        }

  times = TimeChecker();
  NetWorkTask(humi , times);
  
 
delay(2000);  
}
 
