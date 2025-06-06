#include <Arduino.h>
#include <helper.h>
#define echo_limit_left 26
#define echo_limit_right 22
#define trig_limit_left 27
#define trig_limit_right 23

#define SPEED_OMETER 25
#define board_lowered_limit_switch 19
#define board_raised_limit_switch 18
#define DIR1 32
#define DIR2 33
#define Brake_Relay 13 // NO

#define MIN_ADC_SPEED 111
#define MAX_ADC_SPEED 115
#define STOP_DISTANCE_CM 27
#define STOP_DISTANCE_LEFT 17
#define STOP_DISTANCE_RIGHT 28
const int REQUIRED_CONSECUTIVE_SAMPLES = 100;

const unsigned long MAX_UPTIME = 30UL * 60UL * 1000UL; 
unsigned long bootTime = 0;




void setBrakeState(bool on)
{
  digitalWrite(Brake_Relay, on ? HIGH : LOW);
  // Serial.print("Brake: ");
  // Serial.println(on ? "ENGAGED" : "RELEASED");
}

void changeDirection()
{
  setBrakeState(true);
  dacWrite(SPEED_OMETER, 0);
  delay(1000);
  digitalWrite(DIR1, HIGH);
  digitalWrite(DIR2, HIGH);
  delay(2000);
}

long readDistanceCM(int ULTRASONIC_TRIG_PIN, int ULTRASONIC_ECHO_PIN)
{
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  long duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
  long distanceCm = duration * 0.0343 / 2;
  if (distanceCm == 0 || distanceCm > 400)
  {
    return -1;
  }
  return distanceCm;
}

void raiseSpeed(int max)
{
  dacWrite(SPEED_OMETER, 0); // Reset speed to 0 before ramping up
  // Serial.print("Raising speed to DAC value: ");
  // Serial.println(max);
  for (int val = 0; val <= max; val++)
  {
    dacWrite(SPEED_OMETER, val);
    delay(1); // 1ms delay per step
  }
}

void stopMotor()
{
  dacWrite(SPEED_OMETER, 0);
  setBrakeState(true);
  // Serial.println("Motor stopped.");
}

void defactoRelayState()
{
  digitalWrite(DIR1, LOW);
  digitalWrite(DIR2, LOW);
}

void setup()
{
  // Serial.begin(9600);
  pinMode(echo_limit_left, INPUT);
  pinMode(echo_limit_right, INPUT);
  pinMode(trig_limit_left, OUTPUT);
  pinMode(trig_limit_right, OUTPUT);
  pinMode(SPEED_OMETER, OUTPUT);
  pinMode(Brake_Relay, OUTPUT);
  pinMode(DIR1, OUTPUT);
  pinMode(DIR2, OUTPUT);
  pinMode(board_lowered_limit_switch, INPUT);
  pinMode(board_raised_limit_switch, INPUT);

  setBrakeState(true);
  defactoRelayState();
  dacWrite(SPEED_OMETER, 0);
  delay(5000); // ALLOW THE CONTROLLER TO SETTLE
  defactoRelayState();
  stopMotor();
  bootTime = millis();

}

void moveLeft()
{
  int consecutiveMatches = 0;
  for (int i = 0; i < REQUIRED_CONSECUTIVE_SAMPLES; i++)
  {
    if (digitalRead(board_lowered_limit_switch) == LOW && digitalRead(board_raised_limit_switch) == HIGH)
    {
      consecutiveMatches++;
    }
    else
    {
      consecutiveMatches = 0;
    }
    delay(10);
  }

  if (consecutiveMatches < REQUIRED_CONSECUTIVE_SAMPLES)
  {
    return;
  }

  setBrakeState(true);
  dacWrite(SPEED_OMETER, 0);
  long distanceLeft = readDistanceCM(trig_limit_left, echo_limit_left);
  if (distanceLeft >= 0 && distanceLeft <= STOP_DISTANCE_LEFT)
  {
    // Serial.println("Board already left no need to move motor");
    return;
  }
  digitalWrite(DIR1, LOW);
  digitalWrite(DIR2, LOW);
  delay(1000);
  setBrakeState(false);
  raiseSpeed(112);

  unsigned long startTime = millis();
  while (millis() - startTime < 10000)
  {
    long distanceLeft = readDistanceCM(trig_limit_left, echo_limit_left);
    // Serial.print("Distance left: ");
    // Serial.println(distanceLeft);
    dacWrite(SPEED_OMETER, 119);
    if (distanceLeft <= STOP_DISTANCE_LEFT && distanceLeft >= 0)
    {
      // Serial.println("Stopping motor due to limit hit.");
      stopMotor();
      return;
    }

    // Serial.println("Moving left now...");
  }

  // Serial.println("Timeout reached. Stopping.");
  stopMotor();
}

void moveRight()
{

  int consecutiveMatches = 0;
  for (int i = 0; i < REQUIRED_CONSECUTIVE_SAMPLES; i++)
  {
    if (digitalRead(board_raised_limit_switch) == LOW && digitalRead(board_lowered_limit_switch) == HIGH)
    {
      consecutiveMatches++;
    }
    else
    {
      consecutiveMatches = 0;
    }
    delay(10);
  }

  if (consecutiveMatches < REQUIRED_CONSECUTIVE_SAMPLES)
  {
    return;
  }

  setBrakeState(true);
  dacWrite(SPEED_OMETER, 0);
  long distanceRight = readDistanceCM(trig_limit_right, echo_limit_right);
  if (distanceRight <= STOP_DISTANCE_RIGHT && distanceRight >= 0)
  {
    // Serial.println("Board already right no need to move motor");
    return;
  }
  changeDirection();
  setBrakeState(false);
  raiseSpeed(112);

  unsigned long startTime = millis();
  while (millis() - startTime < 10000)
  {
    long distanceRight = readDistanceCM(trig_limit_right, echo_limit_right);
    // Serial.print("Distance right: ");
    // Serial.println(distanceRight);
    // Serial.print("Speed: ");
    dacWrite(SPEED_OMETER, 119);
    if (distanceRight <= STOP_DISTANCE_RIGHT && distanceRight >= 0)
    {
      // Serial.println("Stopping motor due to limit hit.");
      stopMotor();
      return;
    }
    // Serial.println("Moving to right now...");
  }

  // Serial.println("Timeout reached. Stopping.");
  stopMotor();
  return;
}

void loop()
{

  if (digitalRead(board_lowered_limit_switch) == LOW && digitalRead(board_raised_limit_switch) == HIGH)
  {
    moveLeft();
  }

  if (digitalRead(board_raised_limit_switch) == LOW && digitalRead(board_lowered_limit_switch) == HIGH)
  {
    moveRight();
  }


  if (millis() - bootTime > MAX_UPTIME) {
    // Serial.println("Scheduled reboot after 4 hours");
    ESP.restart();
  }
}
