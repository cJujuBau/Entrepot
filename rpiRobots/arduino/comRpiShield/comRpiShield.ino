
#define MAX_CHIFFRE_POS 10

int xMM, yMM;
int xRef, yRef, oRef;
int x, y, O;


void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);  // For communication with the PC (optional)

  // Initialize Serial2
  Serial2.begin(115200);  // Set baud rate for communication with Raspberry Pi

  Serial.print("Serials init\n");
}

void loop() {
  if (Serial2.available()) {
    char data2 = Serial2.read();
    if (data2 == 'p') getPosMM();
    else if (data2 == 'r') getPosRef();
    else if (data2 == '\n' || data2 == '\r') data2 = 0;
    else Serial.write(data2);
    //Serial.write(data2);  // Optional: send data to Serial Monitor for debugging
  }

  if (Serial.available()) {
    char data = Serial.read();
    if (data == 'a') Serial2.write("OuiOuiBagette\n");
    else if (data == 'o') sendObstacleDetected();
    else Serial2.write(data);
    //Serial2.write(data);  // Send data from Serial Monitor to Raspberry Pi
  }

  
  // delay(1000);
}

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
  Serial.print("xMM=");
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
}

void sendObstacleDetected(){
  char cmd[25];
  sprintf(cmd, "o:%d,%d,%d\r\n", xMM, yMM, random(0,361));
  Serial2.write(cmd);
  Serial2.flush();
}
  
