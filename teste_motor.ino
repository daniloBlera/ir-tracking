// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

#include <AFMotor.h>

#define VELOCITY 255     // velocidade de rotaçao das rodas
#define STEP_SIZE 100    // delay em milissegundos

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

int incomingByte = 0;   // for incoming serial data

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("DEMO CONTROLE BT!");

  // Ajusta a velocidade dos motores
  motor1.setSpeed(VELOCITY);
  motor2.setSpeed(VELOCITY);
  motor3.setSpeed(VELOCITY);
  motor4.setSpeed(VELOCITY);
  delay(1000);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);

}

void loop() {
  uint8_t i ;

  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    Serial.print((char)incomingByte);

    switch((char)incomingByte) {
      case 'w':
        motor1.run(FORWARD);
        motor2.run(FORWARD);
        motor3.run(FORWARD);
        motor4.run(FORWARD);
        delay(STEP_SIZE);
        motor1.run(RELEASE);
        motor2.run(RELEASE);
        motor3.run(RELEASE);
        motor4.run(RELEASE);
        break;

      case 's':
        motor1.run(BACKWARD);
        motor2.run(BACKWARD);
        motor3.run(BACKWARD);
        motor4.run(BACKWARD);
        delay(STEP_SIZE);
        motor1.run(RELEASE);
        motor2.run(RELEASE);
        motor3.run(RELEASE);
        motor4.run(RELEASE);
        break;

      case 'a':
        motor1.run(FORWARD);
        motor2.run(FORWARD);
        motor3.run(BACKWARD);
        motor4.run(BACKWARD);
        delay(STEP_SIZE);
        motor1.run(RELEASE);
        motor2.run(RELEASE);
        motor3.run(RELEASE);
        motor4.run(RELEASE);
        break;

      case 'd':
        motor1.run(BACKWARD);
        motor2.run(BACKWARD);
        motor3.run(FORWARD);
        motor4.run(FORWARD);
        delay(STEP_SIZE);
        motor1.run(RELEASE);
        motor2.run(RELEASE);
        motor3.run(RELEASE);
        motor4.run(RELEASE);
        break;

      default:
        motor1.run(RELEASE);
        motor2.run(RELEASE);
        motor3.run(RELEASE);
        motor4.run(RELEASE);
        break;
    }
  }
}
