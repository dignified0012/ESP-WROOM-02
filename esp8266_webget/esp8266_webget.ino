#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

char ssid[] = "D-Home-IoT";
char password[] = "Dpassword";
int sss = 0;

void setup() {
  Serial.begin(115200);
  delay(10);
  
  connectWifi();
}

void loop() {
  getPageSource("http://192.168.50.1:1880/light_bulb1_switch/");
  Serial.println("Switch!");
  sss++;
  delay(1000);
}

void connectWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }

  Serial.println("connected!");
}

void disconnectWifi() {
  WiFi.disconnect();
  Serial.println("disconnected!");
}

String getPageSource(char host[]) {
  HTTPClient http;

  http.begin(host);
  int httpCode = http.GET();

  String result = "";

  if (httpCode < 0) {
    result = http.errorToString(httpCode);
  } else if (http.getSize() < 0) {
    result =  "size is invalid";
  } else {
    result = http.getString();
  }

  http.end();
  return result;
}

