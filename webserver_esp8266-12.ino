#include <ESP8266WiFi.h>


const char* ssid = "your ssid";
const char* password = "youpassword";
int ledPin = 2; // GPIO2
int redPin = 15;  //GPIO15
int greenPin = 12;  //GPIO12
int bluePin = 13;  //GPIO13
int colour;

int UVvalue;
int SolarValue;
char UVString[6];
char  SolarString[6];

WiFiServer server(80);

void setup() {
Serial.begin(115200);
delay(10);

pinMode(ledPin, OUTPUT);
pinMode(redPin, OUTPUT);
pinMode(greenPin, OUTPUT);
pinMode(bluePin, OUTPUT);

pinMode(A0, INPUT); // luminosity

 
digitalWrite(ledPin, LOW);

// Connect to WiFi network
Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);

WiFi.begin(ssid, password);

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
Serial.print("Use this URL to connect: ");
Serial.print("http://");
Serial.print(WiFi.localIP());
Serial.println("/");

}

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

//String request = client.readStringUntil(‘\r’);

//Serial.println(request);


  String req = client.readStringUntil('\r');
  Serial.println(req);
client.flush();

// Match the request

int value = LOW;
if (req.indexOf("/LED=ON") != -1) {
digitalWrite(ledPin, LOW);
value = HIGH;
}
if (req.indexOf("/LED=OFF") != -1) {
digitalWrite(ledPin, HIGH);
value = LOW;
}
if (req.indexOf("/LED=GREEN") != -1) {
//digitalWrite(greenPin, HIGH);
//digitalWrite(bluePin, LOW);
//digitalWrite(redPin, LOW);
       analogWrite(bluePin, 0);
       analogWrite(greenPin, 255);
       analogWrite(redPin, 0);
colour = 1; //green
}
if (req.indexOf("/LED=RED") != -1) {
       analogWrite(bluePin, 0);
       analogWrite(greenPin, 0);
       analogWrite(redPin, 255);
colour = 2; //red
}
if (req.indexOf("/LED=BLUE") != -1) {
       analogWrite(bluePin, 255);
       analogWrite(greenPin, 0);
       analogWrite(redPin, 0);
colour = 3; //red
}

if (req.indexOf("/LED=PURPLE") != -1) {
       analogWrite(bluePin, 255);
       analogWrite(greenPin, 0);
       analogWrite(redPin, 255);
colour = 4; //purple
}






//analogRead(A0)
if (req.indexOf("/Luminosity") != -1) {
  SolarValue = 1024 - analogRead(A0);
  sprintf(SolarString, "%d", SolarValue);
}



// Set ledPin according to the request
//digitalWrite(ledPin, value);

// Return the response
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println(""); // do not forget this one
client.println("<!DOCTYPE HTML>");
client.println("<html>");

client.print("Led pin is now: ");

if(value == HIGH) {
client.print("On");
} else {
client.print("Off");
}


client.println("<br><br>");
client.print("Colour Led is now: ");
 if (colour == 1) {
  client.print("GREEN");
} else if (colour == 2){
  client.print("RED");
} else if (colour == 3){
  client.print("BLUE");
} else if (colour == 4){
  client.print("PURPLE");
}

client.println("<br><br>");
client.print("Luminosity is now: ");
client.println(SolarString);
client.println("<br><br>");
client.println("Click <a href=\"/LED=ON\">here</a> turn the LED on pin 2 ON<br>");
client.println("Click <a href=\"/LED=OFF\">here</a> turn the LED on pin 2 OFF<br>");
client.println("Click <a href=\"/Luminosity\">here</a> to get luminosity reading<br>");
client.println("<br>Change the LED colour<br>");
client.println("Click <a href=\"/LED=RED\">here</a> turn the LED on to RED<br>");
client.println("Click <a href=\"/LED=GREEN\">here</a> turn the LED on to GREEN<br>");
client.println("Click <a href=\"/LED=BLUE\">here</a> turn the LED on to BLUE<br>");
client.println("Click <a href=\"/LED=PURPLE\">here</a> turn the LED on to PURPLE<br>");
client.println("</html>");

delay(1);
Serial.println("Client disonnected");
Serial.println("");

}




