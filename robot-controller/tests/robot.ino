#include "include/MotorController.h"
#include "include/Robot.h"
#include "include/InverseMotorModel.h"
#include "include/Motor.h"
#include "include/Utils.h"

const double SPEED = 150.0; // mm.s-1
const double ROT_SPEED = PI/4.; // rad.s-1

const double Km = 1./25.;
const double Ki = 0.;

const double Kx = 1.;
const double Ky = 1.;

const Point pos_init = Point(0, 0);
const Point pos_ref_1 = Point(500, -500);
const Point pos_ref_2 = Point(50, 300);
const Point pos_ref_3 = Point(200, -300);
const Point pos_ref_4 = Point(500, -300);


const double theta_init = 0;

Motor motorLeft(37, 36, 8, 19, 38, BACKWARD);
Motor motorRight(34, 35, 12, 18, 31, FORWARD);

MotorController motorControllerLeft(Km, Ki);
MotorController motorControllerRight(Km, Ki);

InverseMotorModel inverseMotorModel(Kx, Ky);

Robot robot(pos_init, motorLeft, motorRight, motorControllerLeft, motorControllerRight, inverseMotorModel, theta_init);

void onRisingEdge_MD() {
  motorRight.onRisingEdge();
}

void onRisingEdge_MG() {
  motorLeft.onRisingEdge();
}

void setup() {
  // Initialize the robot
  Serial.begin(115200);
  robot.init();

  attachInterrupt(digitalPinToInterrupt(motorLeft.getVA()), onRisingEdge_MG, RISING);
  attachInterrupt(digitalPinToInterrupt(motorRight.getVA()), onRisingEdge_MD, RISING);
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

  if (elapsedTime <= 16000){
    robot.updateState();

    if (elapsedTime <= 15000) {
      (forwardOnce++ > 0) ? : Serial.println("Forward");
      robot.changeRef(pos_ref_1);

    } else if (elapsedTime <= 15000) {
      (stopOnce++ > 0) ? : Serial.println("Stop");
      robot.changeRef(pos_init);
    }
    //Serial.print("Vg = "); Serial.print(motorLeft.getSpeed()); Serial.print("; Vd = "); Serial.println(motorRight.getSpeed());
    //Serial.print("v = "); Serial.print(robot.getV()); Serial.print("; w = "); Serial.print(robot.getW()); Serial.print("; theta = "); Serial.println(robot.getTheta());
  }
}