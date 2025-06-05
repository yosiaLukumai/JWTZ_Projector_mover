#include <Arduino.h>
#include<helper.h>
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
#define STOP_DISTANCE_LEFT 15



void setBrakeState(bool on)
{
  digitalWrite(Brake_Relay, on ? HIGH : LOW);
  Serial.print("Brake: ");
  Serial.println(on ? "ENGAGED" : "RELEASED");
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
  Serial.print("Raising speed to DAC value: ");
  Serial.println(max);
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
  Serial.println("Motor stopped.");
}


void defactoRelayState()
{
  digitalWrite(DIR1, LOW);
  digitalWrite(DIR2, LOW);
}

// void movetoleftandright()
// {
//   setBrakeState(true);
//   dacWrite(SPEED_OMETER, 0);
//   digitalWrite(DIR1, LOW);
//   digitalWrite(DIR2, LOW);
//   delay(1000);
//   setBrakeState(false);
//   raiseSpeed(112);
//   while (true)
//   {
//     long distanceLeft = readDistanceCM(trig_limit_left, echo_limit_left);

//     Serial.print("Distance left: ");
//     Serial.println(distanceLeft);
//     dacWrite(SPEED_OMETER, (uint8_t)random(113, 115));

//     if (distanceLeft <= STOP_DISTANCE_LEFT && distanceLeft >= 0)
//     {
//       Serial.println("Stopping motor due to limit hit.");
//       stopMotor();
//       break;
//     }

//     // If both sensors are clear, continue moving
//     Serial.println("Moving left now...");
//   }

//   changeDirection();
//   setBrakeState(false);
//   raiseSpeed(112);
//   while (true)
//   {
//     long distanceRight = readDistanceCM(trig_limit_right, echo_limit_right);

//     Serial.print("Distance right: ");
//     Serial.println(distanceRight);
//     uint8_t speed = random(113, 115);
//     Serial.print("Speed: ");
//     Serial.println(speed);

//     dacWrite(SPEED_OMETER, (uint8_t)speed);
//     delay(20);

//     if (distanceRight <= 18 && distanceRight >= 0)
//     {
//       Serial.println("Stopping motor due to limit hit.");
//       stopMotor();
//       break;
//     }

//     // If both sensors are clear, continue moving
//     Serial.println("Moving to right now...");
//   }
// }

void setup()
{
  Serial.begin(9600);
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
}

// void testUltrasonicSensors()
// {
//   long distanceLeft = readDistanceCM(trig_limit_left, echo_limit_left);
//   long distanceRight = readDistanceCM(trig_limit_right, echo_limit_right);
//   Serial.print("Distance left: ");
//   Serial.println(distanceLeft);
//   Serial.print("Distance right: ");
//   Serial.println(distanceRight);
//   if (distanceLeft < 0 || distanceRight < 0)
//   {
//     Serial.println("Ultrasonic sensors are not working");
//     delay(300);
//   }
//   else
//   {
//     Serial.println("Ultrasonic sensors are working");
//   }
//   delay(1000);
// }




void loop()
{


  if (digitalRead(board_lowered_limit_switch) == LOW && digitalRead(board_lowered_limit_switch) != LOW)
  {
    setBrakeState(true);
    dacWrite(SPEED_OMETER, 0);
    long distanceLeft = readDistanceCM(trig_limit_left, echo_limit_left);
    if (distanceLeft >= 0 && distanceLeft <= STOP_DISTANCE_LEFT)
    {
      Serial.println("Board already left no need to move motor");
    }
    else
    {
      digitalWrite(DIR1, LOW);
      digitalWrite(DIR2, LOW);
      delay(1000);
      setBrakeState(false);
      raiseSpeed(112);

      while (true)
      {
        long distanceLeft = readDistanceCM(trig_limit_left, echo_limit_left);
        // uint8_t speed = random(115, 119);
        Serial.print("Distance left: ");
        Serial.println(distanceLeft);
        dacWrite(SPEED_OMETER, 119);
        // dacWrite(SPEED_OMETER, (uint8_t)speed);
        if (distanceLeft <= STOP_DISTANCE_LEFT && distanceLeft >= 0)
        {
          Serial.println("Stopping motor due to limit hit.");
          stopMotor();
          break;
        }

        // If both sensors are clear, continue moving
        Serial.println("Moving left now...");
      }
    }
  }

  if (digitalRead(board_raised_limit_switch) == LOW && digitalRead(board_lowered_limit_switch) == HIGH)
  {
    setBrakeState(true);
    dacWrite(SPEED_OMETER, 0);

    long distanceRight = readDistanceCM(trig_limit_right, echo_limit_right);
    if (distanceRight <= 20 && distanceRight >= 0)
    {
      Serial.println("Board already right no need to move motor");
    }
    else
    {
      changeDirection();
      setBrakeState(false);
      raiseSpeed(112);
      while (true)
      {
        long distanceRight = readDistanceCM(trig_limit_right, echo_limit_right);
        Serial.print("Distance right: ");
        Serial.println(distanceRight);
        // uint8_t speed = random(115, 119);
        Serial.print("Speed: ");
        // Serial.println(speed);
        // dacWrite(SPEED_OMETER, (uint8_t)speed);
        dacWrite(SPEED_OMETER, 119);
        if (distanceRight <= 20 && distanceRight >= 0)
        {
          Serial.println("Stopping motor due to limit hit.");
          stopMotor();
          break;
        }
        // If both sensors are clear, continue moving
        Serial.println("Moving to right now...");
      }
    }
  }


}
