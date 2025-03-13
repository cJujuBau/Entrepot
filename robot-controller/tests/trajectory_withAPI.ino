#include "MeShield.h"
#include <Wire.h>

#include "include/MotorController.h"
#include "include/Robot.h"
#include "include/InverseMotorModel.h"
#include "include/Motor.h"
#include "include/Trajectory.h"
#include "include/Utils.h"

const int MAX_CHIFFRE_POS = 10;

const double Km = 1./50.;
const double Ki = 0.1;

const double Kx = 2.;
const double Ky = 2.;

const Point pos_init = Point(0, 0);
const Point pos_ref = Point(1000, -1500);

MeGyro gyro;

const double theta_init = 0;

int xMM, yMM;
float xRef = -1., yRef = -1., oRef = 0.;

float ti;
float tf = 10000;
float tf2 = 20000;
float tf3 = 30000;
float qi[4]={0.}, qf[4]={0.};

float x_traj, y_traj;
Point pos_traj;

Motor motorLeft(37, 36, 8, 19, 38, BACKWARD);
Motor motorRight(34, 35, 12, 18, 31, FORWARD);

MotorController motorControllerLeft(Km, Ki);
MotorController motorControllerRight(Km, Ki);

InverseMotorModel inverseMotorModel(Kx, Ky);

Robot robot(pos_init, motorLeft, motorRight, motorControllerLeft, motorControllerRight, inverseMotorModel, theta_init);

Trajectory trajectory;

void getPosMM(){
  char strPos[MAX_CHIFFRE_POS+1] = {0};
  int index = 0;
  char recu;

  while(Serial2.available() == 0) delay(1);
  delay(1);
  while(Serial2.available() >0){
    recu = (char) Serial2.read();

    if (recu == ';') {
      strPos[index] = '\0';
      yMM = atoi(strPos); 
      break;
    }
    else if (recu == ':') index = 0;
    else if (recu == ','){
      strPos[index] = '\0';
      xMM = atoi(strPos);
      index=0;
    }
    else if (index >= MAX_CHIFFRE_POS)  {
      Serial.println("getPosMM: Error - Overflow position");
      return;
    }
    else strPos[index++] = recu;
    
  }
  Serial.print("t=");
  Serial.print(millis());
  Serial.print(", xMM=");
  Serial.print(xMM);
  Serial.print(", yMM=");
  Serial.println(yMM);
}

void getPosRef() {
  char strPos[MAX_CHIFFRE_POS + 1] = {0};
  int index = 0;
  char recu;
  int step = 0; // 0 -> xRef, 1 -> yRef, 2 -> oRef

  while (Serial2.available() == 0) delay(1);
  delay(1);

  while (Serial2.available() > 0) {
    recu = (char) Serial2.read();

    if (recu == ';') {
      strPos[index] = '\0';
      oRef = atoi(strPos); // Dernière valeur (oRef)
      break;
    }
    else if (recu == ':') index = 0;
    else if (recu == ',') {
      strPos[index] = '\0'; // Terminer la chaîne
      
      if (step == 0) xRef = atoi(strPos);
      else if (step == 1) yRef = atoi(strPos);
      
      index = 0; // Réinitialisation pour le prochain chiffre
      step++;    // Passer à l'étape suivante
    }
    else if (index >= MAX_CHIFFRE_POS) {
      Serial.println("Overflow position");
      return;
    }
    else {
      strPos[index++] = recu; // Stocker le caractère
    }
  }

  Serial.print("xRef=");
  Serial.print(xRef);
  Serial.print(", yRef=");
  Serial.print(yRef);
  Serial.print(", oRef=");
  Serial.println(oRef);

  qi[0] = robot.getPos().x; qi[1] = robot.getPos().y; qi[2] = robot.getTheta(); qi[3] = millis();
  qf[0] = xRef; qf[1] = yRef; qf[2] = oRef; qf[3] = tf + millis();
  trajectory.computeParameters(qi, qf);
}

void readMM(){
  if (Serial2.available()) {
    char data2 = Serial2.read();
    if (data2 == 'p') getPosMM();
    else if (data2 == 'r') getPosRef();
    else if (data2 == '\n' || data2 == '\r') data2 = 0;
    else Serial.write(data2);
    //Serial.write(data2);  // Optional: send data to Serial Monitor for debugging
    //Serial.print("xMM = "); Serial.print(xMM); Serial.print("yMM = "); Serial.println(yMM);
    robot.readMarvelmind();
  }
}

void onRisingEdge_MD() {
  motorRight.onRisingEdge();
}

void onRisingEdge_MG() {
  motorLeft.onRisingEdge();
}

void setup() {
  // Initialize the robot
  Serial.begin(115200);
  Serial2.begin(115200);
  robot.init();
  gyro.begin();

  attachInterrupt(digitalPinToInterrupt(motorLeft.getVA()), onRisingEdge_MG, RISING);
  attachInterrupt(digitalPinToInterrupt(motorRight.getVA()), onRisingEdge_MD, RISING);

  while ((robot.getPos().x == 0 and robot.getPos().y == 0) or (xRef < 0 and yRef < 0)){
    readMM();
  }
}

void loop() {
  static long timePrec = 0;
  while (millis() - timePrec < DT) {}
  timePrec = millis();


  static unsigned long startTime = millis();
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - startTime;

  readMM();
  
  gyro.update();
  robot.readGyro(theta_init + (gyro.getAngleZ() * PI / 180.));
  //Serial.print("t = "); Serial.print(millis()); Serial.print("; ");
  robot.updateState(); // A modifier: lorsqu'on mesure sur le gyro ou avec MM, il ne faudrait pas mettre à jour l'odométrie
  // Sinon c'est comme si on mettait 2 fois à jour l'état du robot
  

  if (distance(robot.getPos(), pos_ref) >= 100){
    pos_traj = trajectory.computeTraj(robot.getPos().x, millis());
    robot.changeRef(pos_traj);
  //Serial.print("ti = "); Serial.print(ti); Serial.print("; tf = "); Serial.print(tf); Serial.print("; t = "); Serial.println(millis());
  //Serial.print("Vg = "); Serial.print(motorLeft.getSpeed()); Serial.print("; Vd = "); Serial.println(motorRight.getSpeed());
  //Serial.print("v = "); Serial.print(robot.getV()); Serial.print("; w = "); Serial.print(robot.getW()); Serial.print("; theta = "); Serial.println(robot.getTheta());
   //Serial.print("x_traj = "); Serial.print(pos_traj.x); Serial.print("; y_traj = "); Serial.println(pos_traj.y);
  } else {
    motorLeft.setVoltage(0); motorRight.setVoltage(0);
    motorLeft.applyVoltage(); motorRight.applyVoltage();
  }
}
