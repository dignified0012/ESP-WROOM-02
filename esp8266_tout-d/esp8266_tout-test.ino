#include <ESP8266WiFi.h>
extern "C" {
#include "user_interface.h"
}
void setup() {
  Serial.begin(115200);
  Serial.println("PG start");
}

void loop() {

  //unsigned int 型の入れ物を用意
  uint Value = 0;

  //AD変換実行
  Value = system_adc_read();

  //計測結果をシリアルに書き出す。0-1024の値
  if(ADC_Value > 0){Serial.println("ANALOG == "+ String(Value));}
  delay(1);

}
