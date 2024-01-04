#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "ESP32-Access-Point";
const char *password = "12345678";

const int MotorA1 = 17;
const int MotorA2 = 18;
const int MotorB1 = 14;
const int MotorB2 = 12;
const int MotorC1 = 25;
const int MotorC2 = 33;
const int MotorD1 = 27;
const int MotorD2 = 26;


WebServer server(80);

void handleRoot() {
  String html = R"(
<html>
    <head>
        <title>Control</title>
        <script>
            function updateSpeed() {
                var motor1Speed = document.getElementById('motor1Speed').value;
                var motor1Direction = document.getElementById('motor1Direction').value;

                var motor2Speed = document.getElementById('motor2Speed').value;
                var motor2Direction = document.getElementById('motor2Direction').value;

                var motor3Speed = document.getElementById('motor3Speed').value;
                var motor3Direction = document.getElementById('motor3Direction').value;

                var motor4Speed = document.getElementById('motor4Speed').value;
                var motor4Direction = document.getElementById('motor4Direction').value;

                var speedReq = new XMLHttpRequest();

                console.log(motor1Speed);
                console.log(motor2Speed);
                console.log(motor3Speed);
                console.log(motor4Speed);

                speedReq.open(
                    'GET',
                    '/speed?motor1Speed=' + motor1Speed +
                    '&motor1Direction=' + motor1Direction +
                    '&motor2Speed=' + motor2Speed +
                    '&motor2Direction=' + motor2Direction +
                    '&motor3Speed=' + motor3Speed +
                    '&motor3Direction=' + motor3Direction +
                    '&motor4Speed=' + motor4Speed +
                    '&motor4Direction=' + motor4Direction,
                    true
                );
                speedReq.send();
            }
            function turnoff() {
                document.getElementById('motor1Speed').value = 0;
                document.getElementById('motor2Speed').value = 0;
                document.getElementById('motor3Speed').value = 0;
                document.getElementById('motor4Speed').value = 0;
                updateSpeed();
            }
            function turnon() {
                document.getElementById('motor1Direction').value = 'clockwise'
                document.getElementById('motor2Direction').value = 'clockwise'
                document.getElementById('motor3Direction').value = 'clockwise'
                document.getElementById('motor4Direction').value = 'clockwise'
                document.getElementById('motor1Speed').value = 123;
                document.getElementById('motor2Speed').value = 123;
                document.getElementById('motor3Speed').value = 123;
                document.getElementById('motor4Speed').value = 123;
                updateSpeed();
            };
            function getUp() {
                document.getElementById('motor1Direction').value = 'clockwise'
                document.getElementById('motor2Direction').value = 'clockwise'
                document.getElementById('motor3Direction').value = 'clockwise'
                document.getElementById('motor4Direction').value = 'clockwise'
                document.getElementById('motor1Speed').value = 255;
                document.getElementById('motor2Speed').value = 255;
                document.getElementById('motor3Speed').value = 255;
                document.getElementById('motor4Speed').value = 255;
                updateSpeed();
            }
            function goRight() {
                document.getElementById('motor1Speed').value = 255;
                document.getElementById('motor2Speed').value = 123;
                document.getElementById('motor3Speed').value = 255;
                document.getElementById('motor4Speed').value = 123;
                document.getElementById('motor2Direction').value = 'counter-clockwise'
                document.getElementById('motor4Direction').value = 'counter-clockwise'
                updateSpeed()
            }
            function goLeft() {
                document.getElementById('motor1Speed').value = 123;
                document.getElementById('motor2Speed').value = 255;
                document.getElementById('motor3Speed').value = 123;
                document.getElementById('motor4Speed').value = 255;
                document.getElementById('motor1Direction').value = 'counter-clockwise'
                document.getElementById('motor3Direction').value = 'counter-clockwise'
                updateSpeed()
            }
            function forward() {
                document.getElementById('motor1Speed').value = 255;
                document.getElementById('motor2Speed').value = 255;
                document.getElementById('motor3Speed').value = 123;
                document.getElementById('motor4Speed').value = 123;
                document.getElementById('motor4Direction').value = 'counter-clockwise'
                document.getElementById('motor3Direction').value = 'counter-clockwise'
                updateSpeed()
            }
            function backward() {
                document.getElementById('motor1Speed').value = 123;
                document.getElementById('motor2Speed').value = 123;
                document.getElementById('motor3Speed').value = 255;
                document.getElementById('motor4Speed').value = 255;
                document.getElementById('motor1Direction').value = 'counter-clockwise'
                document.getElementById('motor2Direction').value = 'counter-clockwise'
                updateSpeed()
            }
        </script>
        <style>
            .onOffButton {
                padding: 2%;
                border-radius: 0.7em;
                position: fixed;
                cursor: pointer;
                margin-top: 2%;
                transition: all 0.3s ease-in-out;
            }
            
            .onOffButton:hover {
                transform: scale(1.1);
            }
            
            #lean-right,
            #lean-left {
                position: fixed;
                padding: 1%;
                border-radius: 1em;
                cursor: pointer;
                width: 7.5%;
                transition: all 0.3s ease-in-out;
                top: 18dvh;
            }
  
            #lean-left {
                left: 26dvw;
            }
  
            #lean-right {
                right: 29dvw;
            }
  
            .heightData {
                position: fixed;
                margin-top: 8%;
                left: 46dvw;
            }
            
            .upper {
                width: 100vw;
                display: block;
                position: relative;
            }
            
            .upwardControl {
                display: block;
                position: fixed;
                margin-top: 45vh;
                height: 200px;
            }
            
            #speed {
                rotate: 270deg;
                width: 170%;
                transform: scale(1.5);
            }
            
            #forward,
            #backward,
            #up-button,
            #down-button,
            #turn-left,
            #turn-right {
                position: fixed;
                padding: 2.5%;
                cursor: pointer;
                width: 7.5%;
                transition: all 0.3s ease-in-out;
            }
            
            #forward {
                right: 200px;
                top: 300px;
                border-top-right-radius: 80%;
                border-top-left-radius: 80%;
            }
            #backward {
                right: 200px;
                top: 550px;
                border-bottom-left-radius: 80%;
                border-bottom-right-radius: 80%;
            }
            #up-button {
                left: 180px;
                top: 33vh;
                border-top-right-radius: 100%;
                border-top-left-radius: 100%;
            }
            
            #down-button {
                left: 180px;
                border-bottom-right-radius: 80%;
                border-bottom-left-radius: 80%;
            }
            
            #turn-left {
                right: 350px;
                top: 425px;
                border-top-left-radius: 80%;
                border-bottom-left-radius: 80%;
            }
            
            #turn-right {
                right: 50px;
                top: 425px;
                border-top-right-radius: 80%;
                border-bottom-right-radius: 80%;
            }

            #forward:hover,
            #backward:hover,
            #lean-right:hover,
            #lean-left:hover,
            #up-button:hover,
            #down-button:hover,
            #turn-left:hover,
            #turn-right:hover,
            #landButton:hover,
            #flipButton:hover,
            #flyButton:hover {
                transform: scale(1.1);
            }
            
            .camera {
                width: 950px;
                height: 400px;
                top: 250px;
                left: 450px;
                border: 1px solid black;
                border-radius: 0.5em;
                position: fixed;
                background-color: beige;
            }
            
            .bottom {
                display: block;
                position: fixed;
                margin-top: 80vh;
                height: 200px;
                margin-inline-start: 2%;
            }
            
            #landButton,
            #flyButton,
            #flipButton {
                position: fixed;
                padding: 2.5%;
                border-radius: 1em;
                cursor: pointer;
                width: 7.5%;
                transition: all 0.3s ease-in-out;
            }
            
            #landButton {
                left: 10vw;
            }
            
            #flyButton {
                right: 10vw;
            }
            
            #flipButton {
                right: 50vw;
                left: 50vw;
            }  
        </style>
    </head>
    <body>
        <input type='hidden' name='motor1Speed' id='motor1Speed' value='0'>
        <input type='hidden' name='motor1Direction' id='motor1Direction' value='clockwise'>
        <input type='hidden' name='motor2Speed' id='motor2Speed' value='0'>
        <input type='hidden' name='motor2Direction' id='motor2Direction' value='clockwise'>
        <input type='hidden' name='motor3Speed' id='motor3Speed' value='0'>
        <input type='hidden' name='motor3Direction' id='motor3Direction' value='clockwise'>
        <input type='hidden' name='motor4Speed' id='motor4Speed' value='0'>
        <input type='hidden' name='motor4Direction' id='motor4Direction' value='clockwise'>
        <div>
            <div class='upper'>
                <button class='onOffButton' 
                        style='left: 20%; background-color:rgb(31, 212, 31);'
                        type='button' onclick='turnon()'>On
                </button>
                <b class='heightData'>Height: 200 cm</b>
                <button class='onOffButton' 
                        style='right: 20%; background-color:rgb(230, 91, 91)'
                        type='button' onclick='turnoff()'>Off
                </button>
                <button id='lean-left'><b><-</b></button>
                <button id='lean-right'><b>-></b></button>
            </div>
            <div class='upwardControl'>
                <button id='up-button' onmousedown='getUp()' onmouseup='turnon()'>UP</button>
                <button id='down-button'>DOWN</button>
                <button id='forward' onmousedown='forward()' onmouseup='turnon()'>FWD</button>
                <button id='turn-left' onmousedown='goLeft()' onmouseup='turnon()'>LEFT</button>
                <button id='turn-right' onmousedown='goRight()' onmouseup='turnon()'>RIGHT</button>
                <button id='backward' onmousedown='backward()' onmouseup='turnon()'>BWD</button>
                <div class='camera'></div>
            </div>
            <div class='bottom'>
                <button id='landButton'>LAND</button>
                <button id='flipButton'>FLIP</button>
                <button id='flyButton'>FLY</button>
            </div>
        </div>
    </body>
</html>
)";


  server.send(200, "text/html", html);
}

void handleSpeed() {
  if (server.hasArg("motor1Speed") && server.hasArg("motor1Direction") && 
      server.hasArg("motor2Speed") && server.hasArg("motor2Direction") && 
      server.hasArg("motor3Speed") && server.hasArg("motor3Direction") && 
      server.hasArg("motor4Speed") && server.hasArg("motor4Direction")) 
    {
        
        int motor1Speed = server.arg("motor1Speed").toInt();
        int motor2Speed = server.arg("motor2Speed").toInt();
        int motor3Speed = server.arg("motor3Speed").toInt();
        int motor4Speed = server.arg("motor4Speed").toInt();

        String motor1Direction = server.arg("motor1Direction");
        String motor2Direction = server.arg("motor2Direction");
        String motor3Direction = server.arg("motor3Direction");
        String motor4Direction = server.arg("motor4Direction");
    // Control the motor speed

        if (motor1Direction == "clockwise") {
            analogWrite(MotorA1, motor1Speed);
            analogWrite(MotorA2, 0);
        }
        else {
            analogWrite(MotorA1, 0);
            analogWrite(MotorA2, motor1Speed);
        }
        if (motor2Direction == "clockwise") {
            analogWrite(MotorB1, motor1Speed);
            analogWrite(MotorB2, 0);
        }
        else {
            analogWrite(MotorB1, 0);
            analogWrite(MotorB2, motor2Speed);
        }
        if (motor3Direction == "clockwise") {
            analogWrite(MotorC1, motor3Speed);
            analogWrite(MotorC2, 0);
        }
        else {
            analogWrite(MotorC1, 0);
            analogWrite(MotorC2, motor3Speed);
        }
        if (motor4Direction == "clockwise") {
            analogWrite(MotorD1, motor4Speed);
            analogWrite(MotorD2, 0);
        }
        else {
            analogWrite(MotorD1, 0);
            analogWrite(MotorD2, motor4Speed);
        }
        // Send response to the client
        server.send(200, "text/plain", "Speed");
    }
}


void setup() {
  Serial.begin(115200);

  pinMode(MotorA1, OUTPUT);
  pinMode(MotorA2, OUTPUT);
  pinMode(MotorB1, OUTPUT);
  pinMode(MotorB2, OUTPUT);
  pinMode(MotorC1, OUTPUT);
  pinMode(MotorC2, OUTPUT);
  pinMode(MotorD1, OUTPUT);
  pinMode(MotorD2, OUTPUT);

  // Set up the ESP32 as an access point
  WiFi.softAP(ssid, password);

  // Print the IP address assigned to the ESP32 (AP mode)
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // Routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/speed", HTTP_GET, handleSpeed);

  // Start the server
  server.begin();
}

void loop() {
  // Handle client requests
  server.handleClient();
}