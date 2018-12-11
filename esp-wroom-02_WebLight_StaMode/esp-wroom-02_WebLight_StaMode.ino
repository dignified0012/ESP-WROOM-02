#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Ticker.h>

#define Light_Bulb 12
#define Motion_Sensor 14

Ticker ticker;

enum  light_style {light_off = 0, light_on = 1, light_motion = 2};
static enum light_style light_bulb = light_off;

ESP8266WebServer server (80);

static const char *htmldata = "<HTML><BODY style='font-size:80px;'>ESPWROOM02<br/>Light_Bulb<br/><br/><br/><a href=/?light=on>on</a><br/><a href=/?light=off>off</a><br/><a href=/?light=motion>motion</a><br/></BODY></HTML>\r\n";
                              
void handleRoot() {
  String cmd = server.arg("light");
  if (cmd == "on")  
    light_bulb = light_on;
  else if (cmd == "off")
    light_bulb = light_off;
  else if (cmd == "motion")
    light_bulb = light_motion;
  server.send(200, "text/html", htmldata);
}

void function() {
  static byte light_state = 0;
  int motion_state = digitalRead(Motion_Sensor); 

  if (light_bulb == light_off)
    light_state = 0;
  else if (light_bulb == light_on)
    light_state = 1;
  else if (light_bulb == light_motion)
    light_state = motion_state;
  
  digitalWrite(Light_Bulb, light_state);
}

void setup() {
  Serial.begin(115200);
  delay(10);
  
  pinMode(Light_Bulb, OUTPUT);
  pinMode(Motion_Sensor, INPUT);
  
  digitalWrite(Light_Bulb, 0);
  digitalWrite(Motion_Sensor, 1);
  
  ticker.attach_ms(10, function);
  
  Serial.println("WiFi module setting... ");
  WiFi.softAP("SSID", "password");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
  
  Serial.print("Connecting to ");
  WiFi.begin("SSID", "password");
  int retry_sum = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    retry_sum++;
    if(retry_sum > 20) break;
  }
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else{
    Serial.println("WiFi not connected");
  }
  
  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}
