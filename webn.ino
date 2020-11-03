//This example will use a static IP to control the switching of a relay. Over LAN using a web browser. 
//A lot of this code have been resued from the example on the ESP8266 Learning Webpage below. 
//http://www.esp8266learning.com/wemos-webserver-example.php

//CODE START 
//1
#include <ESP8266WiFi.h>

// Below you will need to use your own WIFI informaiton.
//2
const char* ssid = "sahar";               
const char* password = "099224466";     

//defining the pin and setting up the "server"
//3
int RPin = D2;
WiFiServer server(80);
IPAddress ip(192,168 ,43 , 230); // where xx is the desired IP Address
IPAddress gateway(192,168 ,43 , 1); // set gateway to match your network
IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network


// void setup is where we initialize variables, pin modes, start using libraries, etc. 
//The setup function will only run once, after each powerup or reset of the wemos board.
//4
void setup() {
  Serial.begin(115200);
  delay(10);
 

  pinMode(RPin, OUTPUT);
 
  digitalWrite(RPin, LOW);
 
 
  Serial.print(F("Setting static ip to : "));
  Serial.println(ip);
  
  // Connect to WiFi network
  //5
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.config(ip, gateway, subnet); 
  WiFi.begin(ssid, password);
  //Trying to connect it will display dots
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/"); 
}

//void loop is where you put all your code. it is a funtion that returns nothing and will repeat over and over again
//6
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  //Match the request, checking to see what the currect state is
  int value = LOW;
  String led = "No";
  if (request.indexOf("/r-on") != -1) {
    digitalWrite(RPin, HIGH);
   
    value = HIGH;
    led = "Red";
  } 
  /*else if (request.indexOf("/g-on") != -1){
    digitalWrite(RPin, LOW);
    digitalWrite(GPin, HIGH);
    digitalWrite(BPin, LOW);
    value = HIGH;
    led = "Green";
  }
  else if (request.indexOf("/b-on") != -1){
    digitalWrite(RPin, LOW);
    digitalWrite(GPin, LOW);
    digitalWrite(BPin, HIGH);
    value = HIGH;
    led = "Blue";
  }*/
   else if (request.indexOf("/off") != -1){
    digitalWrite(RPin, LOW);

    value = LOW;
    led = "";
  }
  // Return the response, build the html page
  //7
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html style='background-color:#fff;font-family:tahoma'>");
  client.println("<body style='padding:20px;width:500px;margin:50px auto;text-align:center;background-color:#ecf0f1;border:1px solid rgba(0,0,0,0.1);border-radius:3px;overflow:hidden;'>");
 
  client.print("<div>" + led + " LED is now: ");
 
  if(value == HIGH) {
    client.print("ON</div>");  
  } else {
    client.print("OFF</div>");
  }
  client.println("<a href='/r-on' style='color:#fff; background-color: #2ecc71; padding: 20px; margin:20px auto;border-radius:3px;text-decoration:none;display:inline-block;'>Turn Red On</a>");
  client.println("<br>");
  client.println("<a href='/off' style='color:#fff; background-color: #e74c3c; padding: 20px; margin:20px auto;border-radius:3px;text-decoration:none;display:inline-block;'>Turn Off</a>");
  client.println("</body>");
  client.println("</html>");
  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
 
}//END
