#include <SoftwareSerial.h>
#include <Servo.h>

Servo servo01;
Servo servo02;
Servo servo03;
Servo servo04;

SoftwareSerial Bluetooth(3, 4); // Arduino RX, TX - HC-05 Bluetooth TX, RX

int servo1Pos, servo2Pos, servo3Pos, servo4Pos; // Current positions
int servo1PPos = 90, servo2PPos = 90, servo3PPos = 90, servo4PPos = 90; // Previous positions
int speedDelay = 20;

String command = ""; // Command buffer

void setup() {
  servo01.attach(5);
  servo02.attach(6);
  servo03.attach(7);
  servo04.attach(8);

  servo01.write(servo1PPos);
  servo02.write(servo2PPos);
  servo03.write(servo3PPos);
  servo04.write(servo4PPos);

  Bluetooth.begin(38400);
  Serial.begin(38400); // For debugging
}

void loop() {
  // Process incoming data
  while (Bluetooth.available() > 0) {
    String receivedChar = Bluetooth.readString();
    Serial.println(receivedChar);
    if (isCommandComplete(receivedChar)) {
      processCommand(command);
      command = ""; // Clear the command buffer
    } else {
      command += receivedChar; // Append character to command
    }
  }
}

// Check if a command is complete
bool isCommandComplete(char receivedChar) {
  // Command ends when no digits are expected (e.g., length fixed or known patterns)
  if (receivedChar >= '0' && receivedChar <= '9') {
    return false; // Continue receiving digits
  }
  return true; // Otherwise, consider command complete
}

// Process the complete command
void processCommand(String command) {
  Serial.println("Command Received: " + command); // Debug

  if (command.startsWith("s1")) {
    servo1Pos = command.substring(2).toInt();
    moveServo(servo01, servo1PPos, servo1Pos);
    servo1PPos = servo1Pos;
  } else if (command.startsWith("s2")) {
    servo2Pos = command.substring(2).toInt();
    moveServo(servo02, servo2PPos, servo2Pos);
    servo2PPos = servo2Pos;
  } else if (command.startsWith("s3")) {
    servo3Pos = command.substring(2).toInt();
    moveServo(servo03, servo3PPos, servo3Pos);
    servo3PPos = servo3Pos;
  } else if (command.startsWith("s4")) {
    servo4Pos = command.substring(2).toInt();
    moveServo(servo04, servo4PPos, servo4Pos);
    servo4PPos = servo4Pos;
  } else if (command.startsWith("ss")) {
    speedDelay = command.substring(2).toInt();
  } else if (command == "SAVE") {
    savePositions();
  } else if (command == "RUN") {
    runSavedPositions();
  } else if (command == "RESET") {
    resetPositions();
  } else {
    Serial.println("Unknown Command: " + command); // Debug unknown commands
  }
}

// Move a servo from its previous position to the new position
void moveServo(Servo &servo, int prevPos, int newPos) {
  if (prevPos < newPos) {
    for (int i = prevPos; i <= newPos; i++) {
      servo.write(i);
      delay(speedDelay);
    }
  } else {
    for (int i = prevPos; i >= newPos; i--) {
      servo.write(i);
      delay(speedDelay);
    }
  }
}

// Save the current positions
void savePositions() {
  // Example: Save logic here (if needed)
  Serial.println("Positions saved.");
}

// Run the saved positions
void runSavedPositions() {
  // Example: Run logic here (if needed)
  Serial.println("Running saved positions.");
}

// Reset all positions
void resetPositions() {
  servo01.write(90);
  servo02.write(90);
  servo03.write(90);
  servo04.write(90);

  servo1PPos = servo2PPos = servo3PPos = servo4PPos = 90;
  Serial.println("Positions reset.");
}
