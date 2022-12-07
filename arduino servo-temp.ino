#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

const int PIN_LED1 = 2;
const int PIN_LED2 = 22;

/* SSID & Password */
const char* ssid = "Albertito"; //SSID (nombre de la red WiFi)
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
      Serial.print(' . ');
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

String respuestaHtml()
{
  String codigoHtml = "<!DOCTYPE html> <html>\n";
  codigoHtml += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  codigoHtml += "<title>LED Control</title>\n";
  codigoHtml += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  codigoHtml += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  codigoHtml += ".button {display: block;width: 100px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  codigoHtml += ".button-on {background-color: #3498db;}\n";
  codigoHtml += ".button-on:active {background-color: #2980b9;}\n";
  codigoHtml += ".button-off {background-color: #34495e;}\n";
  codigoHtml += ".button-off:active {background-color: #2c3e50;}\n";
  codigoHtml += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  codigoHtml += "</style>\n";
  codigoHtml += "</head>\n";
  codigoHtml += "<body>\n";
  codigoHtml += "<h1>ESP32 Web Server</h1>\n";
  codigoHtml += "<h3>Modo Station (STA)</h3>\n";

  if (estadoLed1 == HIGH)
    codigoHtml += "<p>Estado LED1: ON</p><a class=\"button button-off\" href=\"/led1off\">Apagar</a>\n";
  else
    codigoHtml += "<p>Estado LED1: OFF</p><a class=\"button button-on\" href=\"/led1on\">Encender</a>\n";

    if (estadoLed2 == HIGH)
    codigoHtml += "<p>Estado LED2: ON</p><a class=\"button button-off\" href=\"/led2off\">Apagar</a>\n";
  else
    codigoHtml += "<p>Estado LED2: OFF</p><a class=\"button button-on\" href=\"/led2on\">Encender</a>\n";


  codigoHtml += "</body>\n";
  codigoHtml += "</html>\n";
  return codigoHtml;
}
