// Master //

#include <SoftwareSerial.h> // Include the SoftwareSerial library for serial communication

// Define RX and TX pins for SoftwareSerial
SoftwareSerial BTSerial(4, 5); // RX -> pin 4, TX -> pin 5

// Joystick and potentiometer pins
const int Xjoy1 = A1; // Joystick 1 - X-axis
const int Yjoy1 = A2; // Joystick 1 - Y-axis
const int Xjoy2 = A4; // Joystick 2 - X-axis
const int Yjoy2 = A5; // Joystick 2 - Y-axis
const int Ptmtr = A3; // Potentiometer

const int RecordButtonPin = 5;
const int PlayButtonPin = 4;
const int RedLedPin = 6; // LED indicator for recording

unsigned long t1 = 0, t2 = 0;
const int maxSteps = 100; // Define max steps for recording duration

void setup() {
  // Initialize the primary hardware serial port
  Serial.begin(9600);

  // Initialize the SoftwareSerial port
  BTSerial.begin(9600);

  // Set button pins as inputs
  pinMode(RecordButtonPin, INPUT);
  pinMode(PlayButtonPin, INPUT);

  // Set LED pin as output
  pinMode(RedLedPin, OUTPUT);
}

void loop() {
  unsigned long currenttime = millis();

  // Check if the record button is pressed
  if (digitalRead(RecordButtonPin) == HIGH) {
    if (currenttime - t1 > 1000) { // Debounce to avoid multiple triggers
      BTSerial.println("RECORD");
      Serial.println("Recording started...");
      digitalWrite(RedLedPin, HIGH); // Turn on LED
      t1 = currenttime;
    }
  }

  // Stop recording after a certain time
  if (currenttime - t1 > 80 * maxSteps) { // every steps happens in 80 ms 
    digitalWrite(RedLedPin, LOW);
  }

  // Check if the play button is pressed
  if (digitalRead(PlayButtonPin) == HIGH) {
    if (currenttime - t2 > 1000) { // Debounce
      BTSerial.println("Play");
      Serial.println("Playing recorded movements...");
      t2 = currenttime;
    }
  }

  // Read joystick and potentiometer values
  int joy1xval = analogRead(Xjoy1); // Joystick 1 - X-axis
  int joy1yval = analogRead(Yjoy1); // Joystick 1 - Y-axis
  int joy2xval = analogRead(Xjoy2); // Joystick 2 - X-axis
  int joy2yval = analogRead(Yjoy2); // Joystick 2 - Y-axis
  int ptmtrval = analogRead(Ptmtr); // Potentiometer

  // Send values via Bluetooth Serial
  BTSerial.print(joy1xval);
  BTSerial.print(","); BTSerial.print(joy1yval);
  BTSerial.print(","); BTSerial.print(joy2xval);
  BTSerial.print(","); BTSerial.print(joy2yval);
  BTSerial.print(","); BTSerial.println(ptmtrval);

  delay(80); // Small delay to avoid flooding serial output
}
