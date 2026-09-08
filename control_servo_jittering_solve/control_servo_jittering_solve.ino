#include <Servo.h>
#include <ArduinoQueue.h>
 
ArduinoQueue<int> targetQueue(3);

Servo servo; //Servo 라이브러리 호출 별명 변경

int FlexSensorPin = A0; //flex sensor pin 변수 생성
int ServoMotorPin = 3 ; // servo motor pin 변수 생성

int lastServoAngle = 0;   // 마지막으로 실제 write한 각도 기억
int threshold = 3;        // 데드밴드 폭 (이 값보다 작은 변화는 무시)

void setup() {
  Serial.begin(9600);
  servo.attach(ServoMotorPin);
  servo.write(0); // 전원 연결(시작)시 모터 각도 0
  lastServoAngle = 0;
}

void loop() {
  long FlexSensorValue = analogRead(FlexSensorPin);
  long mean_value = 0;
  
  if (targetQueue.isFull()){
    targetQueue.dequeue();
  }
  targetQueue.enqueue(FlexSensorValue);

  for (int i=0; i < targetQueue.itemCount();i++){
    int item = targetQueue.dequeue();
    mean_value += item;
    targetQueue.enqueue(item);
  }
  mean_value = (mean_value/targetQueue.itemCount());

  long MapValue = map(mean_value, 350, 635, 180, 0);
  MapValue = constrain(MapValue, 0, 180);

  if (MapValue >= 0 && MapValue <= 180){
    if (abs(MapValue - lastServoAngle) >= threshold) {
      servo.write(MapValue);
      lastServoAngle = MapValue;
    }
  }
  else{
    Serial.println("이상치");
  }

  delay(100);
  Serial.print("Raw: ");
  Serial.print(FlexSensorValue);
  Serial.print(" | Mean: ");
  Serial.print(mean_value);
  Serial.print(" | MapValue: ");
  Serial.print(MapValue);
  Serial.print(" | Servo: ");
  Serial.println(lastServoAngle);
}