#include "MotorController.h"
#include "Utils.h"
#include <wiringPi.h>


class MotorController {
public:
    MotorController(int BI1, int BI2, int PWMB, int base_direction);
    void init();
    void setSpeed(int speed);
    void stop();
    //int readSensor();
    void setDirection(int directionToSet);

private:
    int motorSpeed;
    int BI1; // BI1 pin number
    int BI2; // BI2 pin number
    int PWMB; // PWMB pin number
    int base_direction; // rotation direction (1 for direct, -1 for inverse). Depends on the motor physical implementation
    int direction;
}; 

MotorController::MotorController(int BI1, int BI2, int PWMB, int base_direction) : motorSpeed(0), BI1(BI1), BI2(BI2), PWMB(PWMB), base_direction(base_direction), direction(FORWARD) {}

void MotorController::init() {
    // Initialization code for the motors
    wiringPiSetup();
    pinMode(BI1, OUTPUT);
    pinMode(BI2, OUTPUT);
    pinMode(PWMB, PWM_OUTPUT);
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
        digitalWrite8(BI2, HIGH);
    default:
        break;
    }
    direction = directionToSet;
}

void MotorController::setSpeed(int speed) {
    motorSpeed = speed;
    // Code to set the motor speed
    pwmWrite(PWMB, convertToPWM(motorSpeed));
}

void MotorController::stop() {
    setSpeed(0);
}

// // Function to read sensor data
// int MotorController::readSensor() {
//     // Setup wiringPi and set the pin mode
//     wiringPiSetup();
//     int pin = 0; // GPIO pin number (wiringPi pin number)
//     pinMode(pin, INPUT);

//     // Read the value from the pin
//     int value = digitalRead(pin);
//     return value;
// }