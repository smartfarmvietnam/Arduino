#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SHT1x.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <time.h>

WiFiUDP ntpUDP;
//WiFi info 
const char* ssid = "NHALUOI";
const char* password = "khoanndhbl";
//const char* ssid = "PHONGKHACHDHBL";
//const char* password = "dhbl2006";

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
String humid ,temp;
String times;
// 온습도
#define dataPin D10
#define clockPin D11
//릴레이 핀
int relay = D12;

// 온습도 측정
SHT1x sht1x(dataPin, clockPin);

//GMT +7 = 25200
const long utcOffsetInSeconds = 25200;
NTPClient timeClient(ntpUDP , "pool.ntp.org" , utcOffsetInSeconds);

 //현지 시간 체크
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
  void NetWorkTask(String humi, String temp, String times){
    if (WiFi.status() == WL_CONNECTED) { 
       HTTPClient http;  
//       //섹터 A
//       http.begin("http://192.168.3.113:3000/smart_humid_temp_a?humid="+humi+"&temp="+temp+"&date="+times+"");
//       섹터 B
       http.begin("http://192.168.3.113:3000/smart_humid_temp_b?humid="+humi+"&temp="+temp+"&date="+times+"");
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
  temp = sht1x.readTemperatureC();
  humid = sht1x.readHumidity();
  times = TimeChecker();
  NetWorkTask(humid , temp, times);
  int hour =  timeClient.getHours(); 
  int minutes =  timeClient.getMinutes();
  
  Serial.print("현재 시간 체크: ");
  Serial.println(hour);
  Serial.print("현재 분 : ");
  Serial.println(minutes);
  Serial.print("현재 초 : ");
  Serial.println(timeClient.getSeconds());
  Serial.println(temp );
  Serial.println(humid );
 /***
 /**
  *  SetTimes!!!  
  */
     if( hour == 9 || hour == 23) { 
        // 20< minutes < 28
        if( minutes >20  && minutes <30){
           digitalWrite(relay, HIGH); //릴레이 on
           Serial.println("Relay ON!");
          } else { 
             digitalWrite(relay, LOW); //릴레이 off
             Serial.println("Relay OFF!");
         }
     } else{
           digitalWrite(relay, LOW); //릴레이 off
           Serial.println("Relay OFF!");
     }
  delay(2000);  
}
 
