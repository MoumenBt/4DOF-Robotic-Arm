#include <Servo.h>
#include <SoftwareSerial.h>

// Servo objects
Servo servo1, servo2, servo3, servo4, clampServo;

// SoftwareSerial for Bluetooth
SoftwareSerial mySerial(10, 11);
//Tolerance Diffrence In Position
const int positionTolerance = 2;
// Servo pins
const int servo1Pin = 3;
const int servo2Pin = 5;
const int servo3Pin = 6;
const int servo4Pin = 9;
const int clampServoPin = 7;


// Variables for servo positions
int servo1Pos = 90, servo2Pos = 90, servo3Pos = 90, servo4Pos = 90, clampServoPos = 90;
int servo1PrevPos = 90, servo2PrevPos = 90, servo3PrevPos = 90, servo4PrevPos = 90, clampServoPrevPos = 90;
// Maximum recording steps
const int maxSteps = 100;
int recordedMovements[maxSteps][5];
int currentStep = 0;

// Flags for modes
bool isRecording = false;
bool isPlaying = false;
int playIndex = 0;

void setup() {
  // Attach servos
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);
  servo4.attach(servo4Pin);
  clampServo.attach(clampServoPin);

  // Set initial servo positions
  servo1.write(servo1Pos);
  servo2.write(servo2Pos);
  servo3.write(servo3Pos);
  servo4.write(servo4Pos);
  clampServo.write(clampServoPos);

  // Initialize Serial and Bluetooth
  Serial.begin(9600);
  mySerial.begin(9600);

  Serial.println("System ready.");
}

void loop() {
  // Handle Bluetooth commands
  if (mySerial.available()) {
    String command = mySerial.readStringUntil('\n');

    if (command == "RECORD") {
      isRecording = !isRecording; // Toggle recording
      isPlaying = false;          // Stop playback if recording starts
      if (isRecording) {
        currentStep = 0; // Reset recording index
        Serial.println("Recording started.");
      } else {
        Serial.println("Recording stopped.");
      }
    } else if (command == "PLAY") {
      isPlaying = !isPlaying; // Toggle playback
      isRecording = false;    // Stop recording if playback starts
      if (isPlaying) {
        playIndex = 0; // Start from the first step
        Serial.println("Playback started.");
      } else {
        Serial.println("Playback stopped.");
      }
    }
    
  }

  // Handle recording mode
  if (isRecording) {
    if (currentStep < maxSteps) {
      // Store current servo positions
      recordedMovements[currentStep][0] = servo1Pos;
      recordedMovements[currentStep][1] = servo2Pos;
      recordedMovements[currentStep][2] = servo3Pos;
      recordedMovements[currentStep][3] = servo4Pos;
      recordedMovements[currentStep][4] = clampServoPos;
      currentStep++;
      
    } else {
      isRecording = false; // Stop recording if maxSteps reached
      Serial.println("Recording full.");
    }
  }

  // Handle playback mode
  if (isPlaying) {
    if (playIndex < maxSteps) {
      // Move servos to recorded positions
      servo1.write(recordedMovements[playIndex][0]);
      servo2.write(recordedMovements[playIndex][1]);
      servo3.write(recordedMovements[playIndex][2]);
      servo4.write(recordedMovements[playIndex][3]);
      clampServo.write(recordedMovements[playIndex][4]);

      playIndex++;
      delay(80); // Adjust playback speed
    } else {
      isPlaying = false; // Stop playback if all steps are played
      Serial.println("Playback finished.");
    }
  }

  // Read Joystick data input 
  if (!isPlaying) {
    if (mySerial.available()) {
      String data = mySerial.readStringUntil('\n');
      int values[5]; 
      
      convertDataToValues(data, values);

         // Extract joystick and potentiometer values
    int joy1XVal = values[0];
    int joy1YVal = values[1];
    int joy2XVal = values[2];
    int joy2YVal = values[3];
    int potVal   = values[4];

// Update servo positions
    servo1Pos = updateServoPosition(joy1XVal, servo1Pos);
    servo2Pos = updateServoPosition(joy1YVal, servo2Pos);
    servo3Pos = updateServoPosition(joy2XVal, servo3Pos);
    servo4Pos = updateServoPosition(joy2YVal, servo4Pos);
    clampServoPos = constrain(map(potVal, 0, 1023, 0, 180), 0, 180);

   // Constrain servo positions to valid ranges
    servo1Pos = constrain(servo1Pos, 0, 180);
    servo2Pos = constrain(servo2Pos, 0, 180);
    servo3Pos = constrain(servo3Pos, 0, 180);
    servo4Pos = constrain(servo4Pos, 0, 180);

    // Update servos only if position change exceeds tolerance
    if (abs(servo1Pos - servo1PrevPos) > positionTolerance) {
      servo1.write(servo1Pos);
      servo1PrevPos = servo1Pos;
    }
    if (abs(servo2Pos - servo2PrevPos) > positionTolerance) {
      servo2.write(servo2Pos);
      servo2PrevPos = servo2Pos;
    }
    if (abs(servo3Pos - servo3PrevPos) > positionTolerance) {
      servo3.write(servo3Pos);
      servo3PrevPos = servo3Pos;
    }
    if (abs(servo4Pos - servo4PrevPos) > positionTolerance) {
      servo4.write(servo4Pos);
      servo4PrevPos = servo4Pos;
    }
    if (abs(clampServoPos - clampServoPrevPos) > positionTolerance) {
      clampServo.write(clampServoPos);
      clampServoPrevPos = clampServoPos;
    }
        // Debug output
    Serial.println(servo1Pos);
  } else {
    Serial.println("No data received");
  }

  delay(80); // Small delay for stability
    }
  }


// Function to parse data from Bluetooth
void convertDataToValues(String data, int *values) {
  int index = 0, lastComma = -1;
  for (int i = 0; i <= data.length(); i++) {
    if (data.charAt(i) == ',' || i == data.length()) {
      String value = data.substring(lastComma + 1, i);
      values[index++] = value.toInt();
      lastComma = i;
    }
  }
}

// Function to update servo position based on joystick input
int updateServoPosition(int joystickValue, int currentPosition) {
  const int neutralZoneLow = 490;
  const int neutralZoneHigh = 540;
  int addedAngle = abs(joystickValue - 510) * 10 / 512;

  if (joystickValue > neutralZoneHigh) {
    return currentPosition + addedAngle;
  } else if (joystickValue < neutralZoneLow) {
    return currentPosition - addedAngle;
  }
  return currentPosition;
}

