#include <Servo.h>

Servo servo; //Servo 라이브러리 호출 별명 변경

int FlexSensorPin = A0; //flex sensor pin 변수 생성
int ServoMotorPin = 3 ; // servo motor pin 변수 생성

void setup() {
  Serial.begin(9600);
  servo.attach(ServoMotorPin);
  servo.write(0); // 전원 연결(시작)시 모터 각도 0
}

void loop() {
  long FlexSensorValue = analogRead(FlexSensorPin);
  long MapValue = map(FlexSensorValue, 350, 670, 180, 0);

  if (MapValue >= 0 && MapValue <= 180){
    servo.write(MapValue);
  }
  else{
    Serial.println("이상치");
  }
  delay(100);

  Serial.print("Raw: ");
  Serial.print(FlexSensorValue);
  Serial.print(" | Angle: ");
  Serial.println(MapValue);
}