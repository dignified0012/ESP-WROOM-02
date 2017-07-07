#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
 
#define RED_LED 12 
 
Ticker ticker;
 
const char *ssid = "ESP8266AP2";
const char *password = "password";
 
ESP8266WebServer server(80);
 
enum  led_style {led_off = 0, led_on = 1, slow_blink = 2, fast_blink = 3 } ;
static enum led_style red_led = led_off;
long tick_counter = 0;
 
void ticker_func() {
  static byte led_state = 0;
  tick_counter++;
  if (red_led == led_off)
    led_state = 0;
  else if  (red_led == led_on)
    led_state = 1;
  else if ((red_led == slow_blink && (tick_counter % 100 == 0)) || 
      (red_led == fast_blink && (tick_counter % 40 == 0)))
    led_state ^= 1;    
  digitalWrite(RED_LED, led_state);
}
 
static const char* cpResponse400 = "<HTML><BODY>Bad request</BODY></HTML>\r\n";
static const char* cpResponse200 = "<HTML><BODY style='font-size:48px;'>ESP_HTTPD_LED1<br/><br/>"
 "<br/><a href=/cmd?LED=on>on</a><br/><a href=/cmd?LED=off>off</a><br/>"
 "<a href=/cmd?LED=blink1>blink1</a><br/><a href=/cmd?LED=blink2>blink2</a><br/>"
 "</BODY></HTML>\r\n";
 
void send_bad_request() {
  server.send(400, "text/html", cpResponse400);
  Serial.println("Bad request");
}
 
// /cmd?LED=on/off/blink1/blink2
void handleCommand() {
  if (!server.hasArg("LED")) {
      send_bad_request();
      return;
  }
  String cmd = server.arg("LED");
  Serial.println("handleCommand() LED=" + cmd);
  if (cmd == "on")  
    red_led = led_on;
  else if (cmd == "off")
    red_led = led_off;
  else if (cmd == "blink1")
    red_led = slow_blink;
  else if (cmd == "blink2")
    red_led = fast_blink;
  server.send(200, "text/html", cpResponse200);
}
 
void setup() {
  Serial.begin(115200);
  
  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, 0);
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
