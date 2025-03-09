#pragma once

#include "Utils.h"
#include "Arduino.h"

class Motor {
public:
    Motor(int BI1, int BI2, int PWMB, int VA, int VB, int sens);
    void init();

    void setVoltage(const float voltage);
    void applyVoltage();
    float getSpeed();

    int getVA();
    static void handleRisingEdge();
    void onRisingEdge();

private:
    int motorSpeed;
    // Pins used to handle coder
    int VA; // VA pin number
    int VB; // VB pin number

    // Pin used to set speed
    int PWMB; // PWMB pin number

    // Pins used to handle rotation direction
    int BI1; // BI1 pin number
    int BI2; // BI2 pin number

    int sens; // rotation direction (1 for direct, -1 for inverse). Depends on the motor physical implementation

    float u;    // appplied voltage
    float v;    // measured speed
    long pulse; // number of pulse registered since last update
    long pulsePrec; // precedent pulse
    long tempsPrec; // time registered at last update

    static Motor* instance; // Used for interruption

    void updateSpeed();
};