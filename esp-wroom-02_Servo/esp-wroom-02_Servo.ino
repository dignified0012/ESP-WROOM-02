
#include <Servo.h>
Servo servo;

void setup(){
  servo.attach(12);
}

void loop() {
  servo.write(180); //角度を180度に
  delay(1000);
  servo.write(0); //角度を0度に
  delay(1000);
}
