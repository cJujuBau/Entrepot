#include "../include/Motor.h"

Motor::Motor(int BI1, int BI2, int PWMB, int VA, int VB, int sens) : VA(VA), VB(VB), BI1(BI1), BI2(BI2), PWMB(PWMB), sens(sens) {
    this->pulse = 0;
    this->pulsePrec = 0;
    this->tempsPrec = 0;
    this->u = 0;
    this->v = 0;
}

void Motor::init() {
    // Initialization code for the motors
    pinMode(this->VA, INPUT_PULLUP);
    pinMode(this->VB, INPUT_PULLUP);

    pinMode(this->BI1, OUTPUT);
    pinMode(this->BI2, OUTPUT);
    pinMode(this->PWMB, OUTPUT);
}

int Motor::getVA() {
    return VA;
}

int Motor::getVB() {
    return VB;
}

float Motor::getSpeed() {
    updateSpeed();
    return v;
}

void Motor::updateSpeed(){
  //const long tps = millis();
  const long dpulse = (pulse - pulsePrec);
  //const double dt = (tps - tempsPrec);
  double vit = WHEEL_RADIUS * dpulse * 2 * PI;
  vit = vit / (DT * MS_TO_S);
  vit = vit / (PULSE_PER_TURN * REDUCTION_RATE);

  pulsePrec = pulse;
  //tempsPrec = tps;
  v = vit * sens;
}

void Motor::onRisingEdge(){
    if (digitalRead(VB)) {
        pulse++;
    } else {
        pulse--;
    }
}

void Motor::setVoltage(const float voltage) {
    this->u = voltage * sens;
}

void Motor::applyVoltage(){
    float voltage = u;

    if (myAbs(voltage) < 2.5) { voltage = 0; }

    if (voltage > 0) {
        digitalWrite(BI2, LOW);
        digitalWrite(BI1, HIGH);
    } else {
        digitalWrite(BI1, LOW);
        digitalWrite(BI2, HIGH);
    }
    //Serial.print("u = "); Serial.print(u); Serial.print("; voltage = "); Serial.print(voltage); Serial.print("; PWM = "); Serial.println(convertToPWM(myAbs(voltage)));
    analogWrite(PWMB, convertToPWM(myAbs(voltage)));
}
