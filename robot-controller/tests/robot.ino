#include "include/MotorController.h"
#include "include/Robot.h"
#include "include/InverseMotorModel.h"
#include "include/Motor.h"
#include "include/Utils.h"

const double SPEED = 150.0; // mm.s-1
const double ROT_SPEED = PI/4.; // rad.s-1

const double Km = 1./50.;
const double Ki = 0.;

const double Kx = 1.;
const double Ky = 1.;

const Point pos_init = Point(0, 0);
const double theta_init;

Motor motorLeft(34, 35, 12, 18, 31, FORWARD);
Motor motorRight(37, 36, 8, 19, 38, BACKWARD);

MotorController motorControllerLeft(Km, Ki);
MotorController motorControllerRight(Km, Ki);

InverseMotorModel inverseMotorModel(Kx, Ky);

Robot robot(pos_init, motorLeft, motorRight, motorControllerLeft, motorControllerRight, inverseMotorModel, theta_init);

void setup() {
  // Initialize the robot
  Serial.begin(115200);
  robot.init();
}

static int forwardOnce = 0;
static int leftOnce = 0;
static int rightOnce = 0;
static int backwardOnce = 0;
static int stopOnce = 0;

void loop() {
  static long timePrec = 0;
  while (millis() - timePrec < DT) {}
  timePrec = millis();

  static unsigned long startTime = millis();
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - startTime;

  robot.updateState();

  if (elapsedTime <= 3000) {
    (forwardOnce++ == 0) ? : Serial.println("Forward");
    robot.changeRef(SPEED, 0); 

  } else if (elapsedTime <= 6000) {
    (rightOnce++ == 0) ? : Serial.println("Left"); 
    robot.changeRef(0, SPEED);

  } else if (elapsedTime <= 9000) {
    (leftOnce++ > 0) ? : Serial.println("Right");
    robot.changeRef(0, -SPEED);

  } else if (elapsedTime <= 12000) {
    (backwardOnce++ > 0) ? : Serial.println("Backward");
    robot.changeRef(-SPEED, 0);

  } else if (elapsedTime <= 15000) {
    (stopOnce++ > 0) ? : Serial.println("Stop");
    robot.changeRef(0, 0);
  }
}