#include "MotorController.h"
#include "Utils.h"
#include "Arduino.h"

MotorController::MotorController(int BI1, int BI2, int PWMB, int base_direction) : motorSpeed(0), BI1(BI1), BI2(BI2), PWMB(PWMB), base_direction(base_direction) {}

void MotorController::init() {
    // Initialization code for the motors
    pinMode(BI1, OUTPUT);
    pinMode(BI2, OUTPUT);
    pinMode(PWMB, OUTPUT);
}

void MotorController::setDirection(int directionToSet){
    directionToSet *= base_direction; // Inverse la direction si le moteur est monté à l'envers

    switch (directionToSet)
    {
    case FORWARD:
        digitalWrite(BI2, LOW);
        digitalWrite(BI1, HIGH);
        break;
    case BACKWARD:
        digitalWrite(BI1, LOW);
        digitalWrite(BI2, HIGH);
    default:
        break;
    }
}

void MotorController::setSpeed(float speed) {
    if (speed >= 0) setDirection(FORWARD);
    else setDirection(BACKWARD);

    motorSpeed = abs(speed);

    // Code to set the motor speed
    analogWrite(PWMB, convertToPWM(motorSpeed));
}

void MotorController::stop() {
    setSpeed(0);
}

// // Function to read sensor data
// int MotorController::readSensor() {
//     // Setup wiringPi and set the pin mode
//     int pin = 0; // GPIO pin number (wiringPi pin number)
//     pinMode(pin, INPUT);

//     // Read the value from the pin
//     int value = digitalRead(pin);
//     return value;
// }