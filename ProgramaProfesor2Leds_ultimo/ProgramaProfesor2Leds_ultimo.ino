#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <Servo.h>

Servo myservo;

const int PIN_LED1 = 2;
const int PIN_LED2 = 22;
int pos = 0;
/*int servoPin = 18;*/

/* SSID & Password */
const char* ssid = "ESP32"; //SSID (nombre de la red WiFi)
const char* password = "12345678";

//------ Completar acá ------>
IPAddress local_ip(192,168,5,1);
IPAddress gateway(192,168,5,1);
IPAddress subnet(255,255,255,0);
//<---------------------------

WebServer servidor(80);
//WebServer servidor(312);

bool estadoLed1 = LOW;
bool estadoLed2 = LOW;


void setup()
{
  Serial.begin(115200);
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  //------ Completar acá ------>
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
      delay(200);
      Serial.print(" . ");
  }
   //<---------------------------

  servidor.on("/", handle_OnConnect);
  servidor.on("/led1on", handle_led1on);
  servidor.on("/led1off", handle_led1off);
  servidor.on("/led2on", handle_led2on);
  servidor.on("/led2off", handle_led2off);
  servidor.onNotFound(handle_NotFound);

  servidor.begin();
  Serial.println("Inicio servidor HTTP");

  myservo.attach(13);
}

void loop()
{
  servidor.handleClient();
  if (estadoLed1 == true)
    digitalWrite(PIN_LED1, HIGH);
  else
    digitalWrite(PIN_LED1, LOW);
    
    servidor.handleClient();
  if (estadoLed2 == true)
    digitalWrite(PIN_LED2, HIGH);
  else
    digitalWrite(PIN_LED2, LOW);

    for (pos = 0; pos <= 180; pos += 1) { 
   
    myservo.write(pos);
    delay(15);
  }
   for (pos = 180; pos >= 0; pos -= 1) { 
    myservo.write(pos);
    delay(15);
  }
}

void handle_OnConnect()
{
  estadoLed1 = LOW;
  Serial.println("Estado pin 2: OFF");
  estadoLed2 = LOW;
  Serial.println("Estado pin 4: OFF");
  servidor.send(200, "text/html", respuestaHtml());
}

void handle_led1on()
{
  estadoLed1 = HIGH;
  Serial.println("Estado pin 2: ON");
  servidor.send(200, "text/html", respuestaHtml());
}

void handle_led1off()
{
  estadoLed1 = LOW;
  Serial.println("Estado pin 2: OFF");
  servidor.send(200, "text/html", respuestaHtml());
}


void handle_led2on()
{
  estadoLed2 = HIGH;
  Serial.println("Estado pin 4: ON");
  servidor.send(200, "text/html", respuestaHtml());
}

void handle_led2off()
{
  estadoLed2 = LOW;
  Serial.println("Estado pin 4: OFF");
  servidor.send(200, "text/html", respuestaHtml());
}

void handle_NotFound()
{
  servidor.send(404, "text/plain", "Error 404 - Pagina no encontrada");
}

  <!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    body {
      text-align: center;
      font-family: "Trebuchet MS", Arial;
      margin-left:auto;
      margin-right:auto;
    }
    .slider {
      width: 300px;
    }
  </style>
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
</head>
<body>
  <h1>ESP32 with Servo</h1>
  <p>Position: <span id="servoPos"></span></p>
  <input type="range" min="0" max="180" class="slider" id="servoSlider" onchange="servo(this.value)"/>
  <script>
    var slider = document.getElementById("servoSlider");
    var servoP = document.getElementById("servoPos");
    servoP.innerHTML = slider.value;
    slider.oninput = function() {
      slider.value = this.value;
      servoP.innerHTML = this.value;
    }
    $.ajaxSetup({timeout:1000});
    function servo(pos) {
      $.get("/?value=" + pos + "&");
      {Connection: close};
    }
  </script>
</body>
</html>
