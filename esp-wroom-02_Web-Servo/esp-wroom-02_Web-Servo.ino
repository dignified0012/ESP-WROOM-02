#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>

#define Servo_PWM 12

Servo servo;
Ticker ticker;

const char *ssid = "ESP-WROOM-02";
const char *password = "12345678";

ESP8266WebServer server(80);

enum  ServoAngle {servo_0 = 0, servo_90 = 90, servo_180 = 180 };
static enum ServoAngle servo_sg90 = servo_0;
long tick_counter = 0;

void ticker_func() {
  static byte servo_state = 0;
  tick_counter++;
  if (servo_sg90 == servo_0)
    servo_state = 0;
  else if  (servo_sg90 == servo_90)
    servo_state = 90;
  else if (servo_sg90 == servo_180)
    servo_state = 180;
  servo.write(servo_state);
}

static const char* cpResponse400 = "<HTML><BODY>Bad request</BODY></HTML>\r\n";
static const char* cpResponse200 = "<HTML><BODY style='font-size:48px;'>ESP_HTTPD_SERVO<br/><br/>"
 "<br/><a href=/cmd?servo=0>0</a><br/><a href=/cmd?servo=90>90</a><br/><a href=/cmd?servo=180>180</a><br/>"
 "</BODY></HTML>\r\n";
 
 void send_bad_request() {
  server.send(400, "text/html", cpResponse400);
  Serial.println("Bad request");
}

void handleCommand() {
  if (!server.hasArg("Servo")) {
      send_bad_request();
      return;
  }
  String cmd = server.arg("Servo");
  Serial.println("handleCommand() Servo=" + cmd);
  if (cmd == "0")  
    servo_sg90 = servo_0;
  else if (cmd == "90")
    servo_sg90 = servo_90;
  else if (cmd == "180")
    servo_sg90 = servo_180;
  server.send(200, "text/html", cpResponse200);
}

void setup(){
  Serial.begin(115200);
  
  servo.attach(Servo_PWM);
  ticker.attach_ms(10, ticker_func);
 
  WiFi.softAP(ssid, password);
  IPAddress ip = WiFi.softAPIP();
  Serial.println("");
  Serial.println(ssid + String(" starts..")); 
  Serial.print("this AP : "); 
  Serial.println(ip);
 
  server.on("/cmd", handleCommand);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  delay(1);
}
