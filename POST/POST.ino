#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

void setup() {
 
  Serial.begin(115200);                                  //Serial connection
  WiFi.begin("GGUEGGUE", "11118888");   //WiFi connection
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 
    delay(500);
    Serial.println("Waiting for connection");
 
  }
 
}
 
void loop() {
 
 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
 
   HTTPClient http;    //Declare object of class HTTPClient
 
   http.begin("http://192.168.43.223:3000/humid?humid=11&humid_date=2019.05.11/09:00:00");      //Specify request destination
   http.addHeader("Content-Type", "text/plain");  //Specify content-type header
//   http.println(humi);
//   http.println(dateTime);
   int httpCode = http.GET();   //Send the request
   String payload = http.getString();                  //Get the response payload
 
   Serial.println(httpCode);   //Print HTTP return code
   Serial.println(payload);    //Print request response payload
 
   http.end();  //Close connection
 
 }else{
 
    Serial.println("Error in WiFi connection");   
 
 }
 
  delay(30000);  //Send a request every 30 seconds
 
}
