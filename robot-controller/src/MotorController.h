#pragma once

#define FORWARD 1
#define BACKWARD -1

class Motor {
public:
    Motor(int BI1, int BI2, int PWMB, int VA, int VB, int sens);
    void init();

    void setVoltage(const float voltage);
    void applyVoltage();
    float getSpeed();

private:
    int motorSpeed;
    int BI1; // BI1 pin number
    int BI2; // BI2 pin number
    int PWMB; // PWMB pin number
    int VA; // VA pin number
    int VB; // VB pin number
    int sens; // rotation direction (1 for direct, -1 for inverse). Depends on the motor physical implementation

    float u;
    float v;
    long pulse;
    long pulsePrec;
    long tempsPrec;

    static Motor* instance;

    static void onRisingEdge();
    void handleRisingEdge();

    void updateSpeed();
};

class MotorController {
public:
    MotorController(double Km, double Ki, double ref);
    void setReference(double ref);
    void updateOutput(Motor &motor);

private:
    double Km;
    double Ki;
    double ref;
};