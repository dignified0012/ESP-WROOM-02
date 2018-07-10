#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

//extern "C" {
//  #include "user_interface.h"
//}

char ssid[] = "D-Home-IoT";
char password[] = "Dpassword";

void setup() {
  Serial.begin(115200);
  delay(10);
  connectWifi();
}

void loop() {
  //unsigned int 型の入れ物を用意
//  uint ADC_Value = 0;
//  //AD変換実行
//  ADC_Value = system_adc_read();

  int ADC_Value = analogRead(A0);
  //Serial.println(ADC_Value);

  //計測結果をシリアルに書き出す。0-1024の値
  //10以上ならknockにカウント
  if(ADC_Value > 10) {
    Serial.println("ANALOG == "+ String(ADC_Value));
    getPageSource("http://192.168.50.1:1880/light_bulb1_switch/");
    delay(1000);
    //knock++;
  }
  //delayMicroseconds(10) //マイクロ秒数
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

