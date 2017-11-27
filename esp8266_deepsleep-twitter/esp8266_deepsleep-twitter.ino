#include <Time.h>
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <EasyNTPClient.h>

const char* ssid = "###############";
const char* password = "###############";

const char* host = "stewgate-u.appspot.com";
const char* token = "###############";

unsigned int localPort = 2390; 
WiFiUDP udp;
EasyNTPClient ntpClient(udp, "jp.pool.ntp.org", ((9*60*60)+(0*60)));

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  udp.begin(localPort);
}

void loop() {
  setTime(ntpClient.getUnixTime());
  delay(1000);
  
  String message = "ドア開いたよ　JST : ";
  message += year();
  message += "/";
  message += month();
  message += "/";
  message += day();
  message += " ";
  message += hour();
  message += ":";
  message += minute();
  message += ":";
  message += second();
  
  boolean result = tweetMsg(message);
  
  Serial.print("Result = ");
  Serial.println(result);

  if(result){
    Serial.println("DeepSleepMode...");
    ESP.deepSleep(0);
    delay(1000);
  }
}


bool tweetMsg(String msg) {
  WiFiClient client;
  const int httpPort = 80;
  if(!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return false;
  }

  client.println("POST /api/post/ HTTP/1.0");

  client.print("Host: ");
  client.println(host);

  int msgLength = 40;
  msgLength += msg.length();
  client.print("Content-length:");
  client.println(msgLength);
  client.println("");
  client.print("_t=");
  client.print(token);
  client.print("&msg=");
  client.println(msg);

  delay(10);

  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  return true;
}
