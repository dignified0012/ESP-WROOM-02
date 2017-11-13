#include <ESP8266WiFi.h>
extern "C" {
#include "user_interface.h"
}

void setup() {
  //デバッグ用にシリアルを開く
  Serial.begin(115200);
  Serial.println("PG start");
  pinMode(12, OUTPUT);
}

void loop() {
  //unsigned int 型の入れ物を用意
  uint ADC_Value = 0;

  //AD変換実行
  ADC_Value = system_adc_read();

  //計測結果をシリアルに書き出す。0-1024の値
  //Serial.println("=======ANALOG " + String(ADC_Value) + "ANALOG ");
  if(ADC_Value > 480){digitalWrite(12, HIGH);}
  else{digitalWrite(12, LOW);}
  delay(1);
}
