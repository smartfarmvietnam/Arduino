#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <time.h>

const char* ssid = "GGUEGGUE";
const char* password = "11118888";

String formattedDate;
WiFiUDP ntpUDP;

//GMT +7 = 25200
const long utcOffsetInSeconds = 25200;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
NTPClient timeClient(ntpUDP , "pool.ntp.org" , utcOffsetInSeconds);

 void SensingLCD(int moisture){
  Serial.print("Moisture Sensor Value:");
  Serial.println(moisture);
//  lcd.setCursor(0, 0);
//  lcd.print("Moisture: ");
//  lcd.print(moisture);
//  lcd.setCursor(0, 1);
//  lcd.print(map(moisture, 1000 ,500 ,0,100));
//  lcd.print("%");
  delay(2000);
} 
void setup () {

Serial.begin(115200);
WiFi.begin(ssid, password);
 
while (WiFi.status() != WL_CONNECTED) {
delay(1000);
Serial.print(".");
}
 Serial.println("연결 완료!!");
  Serial.print("IP address: ");
 Serial.println(WiFi.localIP());
 timeClient.begin();

}

String humi;
void loop() {
////  time_t now = time(nullptr);
////  String date = ctime(&now);
//    time_t t = now();
//    Serial.println(ctime(t));
//    Serial.println(t);
//  Serial.println(ctime(&now));
  int moisture =(map(analogRead(0), 1000 ,500 ,0,100));
  humi = (String)moisture;
  Serial.println("형변환 습도 :" + humi + "%");

 
if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 timeClient.update();
 const char* day = daysOfTheWeek[timeClient.getDay()];
 String hour =(String) timeClient.getHours() + ":";
 String minutes =(String) timeClient.getMinutes()+ ":";
 String seconds =  (String)timeClient.getSeconds();

 String times = hour + minutes + seconds;
 HTTPClient http;  
 http.begin("http://192.168.43.223:3000/humid?humid="+humi+"&humid_date="+times+"");
 int httpCode = http.GET();                                                                 
 
if (httpCode == HTTP_CODE_OK) { 
 Serial.print("HTTP response code ");
 Serial.println(httpCode); 
 String response = http.getString();   
 Serial.println(response);        
 timeClient.update();
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());       
  
 
}
http.end();   //Close connection
 
}else{
  Serial.println("Do not Request");
}
 
delay(2000);  
}//Send a request every 30 seconds
 
