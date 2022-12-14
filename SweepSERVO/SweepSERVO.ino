#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <Servo.h>


//#define DHTTYPE DHT11 

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int servoPin = 13;
//uint8_t DHTPin = 4; 

const char* ssid = "DT"; 
const char* password = "12345678";

IPAddress local_ip(192,168,5,1);
IPAddress gateway(192,168,5,1);
IPAddress subnet(255,255,255,0);

WebServer servidor(80);
 
int pos = 0;
int pos2 = 170;

//DHT dht(DHTPin, DHTTYPE);  

//float Temperature;
//float Humidity;

void setup()
{
  Serial.begin(115200);
  myservo.attach(servoPin);
  //pinMode(DHTPin(4), INPUT);

  //dht.begin();    
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
      delay(200);
      Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  servidor.on("/", handle_OnConnect);
  servidor.on("/servo0", handle_servoMinimo);
  servidor.on("/servo170", handle_servoMaximo);
  servidor.onNotFound(handle_NotFound);

  servidor.begin();
  Serial.println("Inicio servidor HTTP");
}

void loop() {

  servidor.handleClient();
}

void handle_OnConnect()
{
  int pos = 0;
  Serial.println("Estado pos min: OFF");
  int pos2 = 170;
  Serial.println("Estado pos max: OFF");
  servidor.send(200, "text/html", respuestaHtml());
}

//void handle_OnConnectSensor(){
  
  //Temperature = dht.readTemperature(); 
  //Humidity = dht.readHumidity();  
  //server.send(200, "text/html", SendHTML(Temperature,Humidity)); 
//}

void handle_servoMinimo()
{  
    myservo.write(0);  
    servidor.send(200, "text/html", respuestaHtml());
}

void handle_servoMaximo()
{
    myservo.write(170);  
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
  codigoHtml += "<title>ET36 WEBSERVER SERVO/DHT11</title>\n";
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
  codigoHtml += "<h1>ESP32 Web Server FLORES/FORTUNATO</h1>\n";
  codigoHtml += "<h3>Modo Station (STA)</h3>\n";

  if (pos == LOW)
    codigoHtml += "<p>SERVO INICIAL: Off</p><a class=\"button button-off\" href=\"/servo0\">MINIMO SERVO</a>\n";
  else 
    codigoHtml += "<p>SERVO INICIAL: Off</p><a class=\"button button-off\" href=\"/servo0\">MINIMO SERVO</a>\n";

  if (pos2 == LOW)
    codigoHtml += "<p>SERVO MAXIMO: Off</p><a class=\"button button-on\" href=\"/servo170\">MAXIMO SERVO</a>\n";
  else 
    codigoHtml += "<p>SERVO MAXIMO: Off</p><a class=\"button button-on\" href=\"/servo170\">MAXIMO SERVO</a>\n";
    
  codigoHtml += "</body>\n";
  codigoHtml += "</html>\n";
  return codigoHtml;
}
