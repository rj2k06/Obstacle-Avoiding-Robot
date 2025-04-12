#include <Servo.h>          // Servo motor library
#include <NewPing.h>         // Ultrasonic sensor function library

// Motor control pins
const int LeftMotorForward = 7;
const int LeftMotorBackward = 6;
const int RightMotorForward = 5;
const int RightMotorBackward = 4;

// LED control pins
const int ForwardLED = 8;
const int BackwardLED = 9;
const int LeftLED = 11;
const int RightLED = 12;
const int LeftSensorLED = 13;
const int RightSensorLED = 3;

// Ultrasonic sensor pins and settings
#define trig_pin A1
#define echo_pin A2
#define maximum_distance 200
#define obstacle_distance 48   // Reduced threshold distance to detect an obstacle
NewPing sonar(trig_pin, echo_pin, maximum_distance);

// Servo motor setup
Servo servo_motor;

// Servo rotation range
int servoLeftPos = 50;
int servoRightPos = 170;
int servoNeutral = 115; // Neutral position in the center

// Function prototypes
int readPing();
void moveStop();
void moveForward();
void moveBackward();
void turnRight();
void turnLeft();
int lookRight();
int lookLeft();

void setup() {
  // Set motor control pins to OUTPUT
  pinMode(RightMotorForward, OUTPUT);
  pinMode(LeftMotorForward, OUTPUT);
  pinMode(LeftMotorBackward, OUTPUT);
  pinMode(RightMotorBackward, OUTPUT);

  // Set LED pins to OUTPUT
  pinMode(ForwardLED, OUTPUT);
  pinMode(BackwardLED, OUTPUT);
  pinMode(LeftLED, OUTPUT);
  pinMode(RightLED, OUTPUT);
  pinMode(LeftSensorLED, OUTPUT);
  pinMode(RightSensorLED, OUTPUT);

  // Initialize servo
  servo_motor.attach(10);
  servo_motor.write(servoNeutral);  // Start at the neutral position
  delay(500);

  // Start moving forward initially
  moveForward();
}

void loop() {
  // Always scan with the servo motor
  scanEnvironment();

  int distance = readPing();

  if (distance <= obstacle_distance) {
    // Stop and decide on a new direction if obstacle is detected
    moveStop();
    delay(300);
    moveBackward();
    delay(400);
    moveStop();
    delay(300);

    int distanceRight = lookRight();
    int distanceLeft = lookLeft();

    if (distanceRight >= distanceLeft) {
      turnRight();
    } else {
      turnLeft();
    }
    moveForward();  // Continue moving forward after turning
  }
}

void scanEnvironment() {
  // Continuously scan by moving the servo motor between two positions
  static unsigned long lastScanTime = 0;
  unsigned long currentMillis = millis();

  // Make sure the servo scans smoothly at a regular interval
  if (currentMillis - lastScanTime >= 500) {
    // Move the servo between left and right positions
    static bool movingLeft = true;

    if (movingLeft) {
      servo_motor.write(servoLeftPos);  // Move to the left
    } else {
      servo_motor.write(servoRightPos);  // Move to the right
    }
    movingLeft = !movingLeft;  // Toggle between left and right positions

    lastScanTime = currentMillis;  // Update the last scan time
  }
}

int lookRight() {
  digitalWrite(RightSensorLED, HIGH);
  delay(200);
  digitalWrite(RightSensorLED, LOW);

  // Check the distance at the right
  int distance = readPing();
  delay(100);  // Give some time to stabilize the reading

  return distance;
}

int lookLeft() {
  digitalWrite(LeftSensorLED, HIGH);
  delay(200);
  digitalWrite(LeftSensorLED, LOW);

  // Check the distance at the left
  int distance = readPing();
  delay(100);  // Give some time to stabilize the reading

  return distance;
}

int readPing() {
  delay(70);
  int cm = sonar.ping_cm();
  return (cm == 0) ? maximum_distance : cm;
}

void moveStop() {
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
  digitalWrite(LeftMotorBackward, LOW);

  digitalWrite(ForwardLED, LOW);
  digitalWrite(BackwardLED, LOW);
  digitalWrite(LeftLED, LOW);
  digitalWrite(RightLED, LOW);
}

void moveForward() {
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW);
  digitalWrite(ForwardLED, HIGH);
}

void moveBackward() {
  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(BackwardLED, HIGH);
}

void turnRight() {
  digitalWrite(RightLED, HIGH);
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorForward, LOW);
  delay(250);

  digitalWrite(RightLED, LOW);
}

void turnLeft() {
  digitalWrite(LeftLED, HIGH);
  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
  delay(250);

  digitalWrite(LeftLED, LOW);
}
