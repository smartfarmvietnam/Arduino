#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <time.h>

const char* ssid = "GGUEGGUE";
const char* password = "11118888";

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
Serial.println("Connecting..");
 
}
 Serial.println("연결 완료!!");
 Serial.println(WiFi.localIP());
}

String humi;
void loop() {
  time_t now = time(nullptr);
  String date = ctime(&now);
  Serial.println(ctime(&now));
  int moisture =(map(analogRead(0), 1000 ,500 ,0,100));
  humi = (String)moisture;
  Serial.println("형변환 습도 :" + humi + "%");

 
if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
HTTPClient http;  //Declare an object of class HTTPClient
 
 http.begin("http://192.168.43.223:3000/humid?humid="+humi+"&humid_date="+date+"");
 //Specify request destination
int httpCode = http.GET();                                                                  //Send the request
 
if (httpCode > 0) { //Check the returning code
 
String payload = http.getString();   //Get the request response payload
Serial.println(payload);                     //Print the response payload
 
}
 
http.end();   //Close connection
 
}
 
delay(5000);  
}//Send a request every 30 seconds
 
