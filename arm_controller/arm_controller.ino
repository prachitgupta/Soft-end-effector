/*        
   DIY Arduino Robot Arm Smartphone Control  
   by Dejan, www.HowToMechatronics.com  
*/

#include <SoftwareSerial.h>
#include <Servo.h>

Servo servo01;
Servo servo02;
Servo servo03;
Servo servo04;

SoftwareSerial Bluetooth(3, 4); // Arduino(RX, TX) - HC-05 Bluetooth (TX, RX)

int servo1Pos, servo2Pos, servo3Pos, servo4Pos; // current position
int servo1PPos, servo2PPos, servo3PPos, servo4PPos; // previous position
int servo01SP[50], servo02SP[50], servo03SP[50], servo04SP[50]; // for storing positions/steps
int speedDelay = 20;
int index = 0;
String dataIn = "";

void setup() {
  servo01.attach(5);
  servo02.attach(6);
  servo03.attach(7);
  servo04.attach(8);
 
  Bluetooth.begin(9600); // Default baud rate of the Bluetooth module
  Serial.begin(9600);
  Bluetooth.setTimeout(1);
  delay(20);

  // Robot arm initial position
  servo1PPos = 90;
  servo01.write(servo1PPos);
  servo2PPos = 90;
  servo02.write(servo2PPos);
  servo3PPos = 90;
  servo03.write(servo3PPos);
  servo4PPos = 90;
  servo04.write(servo4PPos);
}

void loop() {
  // Check for incoming data
  if (Bluetooth.available() > 0) {
    dataIn = Bluetooth.readString();  // Read the data as string
    Serial.println("Received Data: " + dataIn);
    
    // If "Waist" slider has changed value - Move Servo 1 to position
    if (dataIn.startsWith("s1")) {
      String dataInS = dataIn.substring(2);
      servo1Pos = dataInS.toInt();
      moveServo(servo01, servo1PPos, servo1Pos);
      servo1PPos = servo1Pos;
    }

    // Move Servo 2
    if (dataIn.startsWith("s2")) {
      String dataInS = dataIn.substring(2);
      servo2Pos = dataInS.toInt();
      moveServo(servo02, servo2PPos, servo2Pos);
      servo2PPos = servo2Pos;
    }

    // Move Servo 3
    if (dataIn.startsWith("s3")) {
      String dataInS = dataIn.substring(2);
      servo3Pos = dataInS.toInt();
      moveServo(servo03, servo3PPos, servo3Pos);
      servo3PPos = servo3Pos;
    }

    // Move Servo 4
    if (dataIn.startsWith("s4")) {
      String dataInS = dataIn.substring(2);
      servo4Pos = dataInS.toInt();
      moveServo(servo04, servo4PPos, servo4Pos);
      servo4PPos = servo4Pos;
    }

    // Adjust Speed
    if (dataIn.startsWith("ss")) {
      String dataInS = dataIn.substring(2);
      speedDelay = dataInS.toInt();
    }
    
    // If button "SAVE" is pressed
    if (dataIn.startsWith("SAVE")) {
      servo01SP[index] = servo1PPos;
      servo02SP[index] = servo2PPos;
      servo03SP[index] = servo3PPos;
      servo04SP[index] = servo4PPos;
      index++; 
    }

    // If button "RUN" is pressed
    if (dataIn.startsWith("RUN")) {
      runservo();
    }

    // If button "RESET" is pressed
    if (dataIn == "RESET") {
      memset(servo01SP, 0, sizeof(servo01SP));
      memset(servo02SP, 0, sizeof(servo02SP));
      memset(servo03SP, 0, sizeof(servo03SP));
      memset(servo04SP, 0, sizeof(servo04SP));
      index = 0;
    }

    dataIn = "";  // Clear the data buffer
  }
}

// Move a servo from its previous position to the new position
void moveServo(Servo &servo, int prevPos, int newPos) {
  if (prevPos < newPos) {
    for (int i = prevPos; i <= newPos; i++) {
      servo.write(i);
      delay(speedDelay);
    }
  } else if (prevPos > newPos) {
    for (int i = prevPos; i >= newPos; i--) {
      servo.write(i);
      delay(speedDelay);
    }
  }
}

// Automatic mode: run the saved steps
void runservo() {
  while (dataIn != "RESET") {
    for (int i = 0; i < index - 1; i++) {
      moveServo(servo01, servo01SP[i], servo01SP[i + 1]);
      moveServo(servo02, servo02SP[i], servo02SP[i + 1]);
      moveServo(servo03, servo03SP[i], servo03SP[i + 1]);
      moveServo(servo04, servo04SP[i], servo04SP[i + 1]);
    }
  }
}
