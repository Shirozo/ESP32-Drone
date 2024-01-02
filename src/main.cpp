#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "ESP32-Access-Point";
const char *password = "12345678";

const int IN1 = 17;
const int IN2 = 18;
const int IN3 = 26;
const int IN4 = 27;

WebServer server(80);

void handleRoot() {
  String html = "<html><head><script>"
                "function updateSpeed() {"
                "  var speed = document.getElementById('speed').value;"
                "  var xhr = new XMLHttpRequest();"
                "  xhr.open('GET', '/speed?speed=' + speed, true);"
                "  xhr.send();"
                "};"
                "function turnoff() {"
                  "var offReq = new XMLHttpRequest();"
                  "offReq.open('GET', '/off');"
                  "offReq.send();"
                "};"
                "function turnon() {"
                  "var onReq = new XMLHttpRequest();"
                  "onReq.open('GET', '/on');"
                  "onReq.send();"
                "};"
                "</script></head><body>";
  html += "<h1>Motor Speed Control</h1>";
  html += "<input type='range' id='speed' name='speed' min='0' max='255' onchange='updateSpeed()'>";
  html += "<output for='speed' id='speedOutput'>0</output><br>";
  html += "<button type='button' onclick='turnoff()'>Turn Off</button><br>";
  html += "<button type='button' onclick='turnon()'>Turn On</button>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleSpeed() {
  if (server.hasArg("speed")) {
    int speed = server.arg("speed").toInt();

    // Control the motor speed
    analogWrite(IN1, speed);
    digitalWrite(IN2, LOW);
    analogWrite(IN3, speed);
    digitalWrite(IN4, LOW); 

    // Send response to the client
    server.send(200, "text/plain", "Speed set to: " + String(speed));
  }
}

void handleOff() {
  analogWrite(IN1, 0);
  digitalWrite(IN2, LOW);
  analogWrite(IN3, 0);
  digitalWrite(IN4, LOW); 

  server.send(200, "text/plain", "Motor Turn Off");
}

void handleOn() {
  analogWrite(IN1, 123);
  digitalWrite(IN2, LOW);
  analogWrite(IN3, 123);
  digitalWrite(IN4, LOW); 

  server.send(200, "text/plain", "Motor Turn On");
}

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Set up the ESP32 as an access point
  WiFi.softAP(ssid, password);

  // Print the IP address assigned to the ESP32 (AP mode)
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // Routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/speed", HTTP_GET, handleSpeed);
  server.on("/off", HTTP_GET, handleOff);
  server.on("/on", HTTP_GET, handleOn);

  // Start the server
  server.begin();
}

void loop() {
  // Your motor control code here
  delay(3000);

  // Handle client requests
  server.handleClient();
}