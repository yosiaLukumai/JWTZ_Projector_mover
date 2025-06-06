#include <Arduino.h>

void TestLimitSwitches(uint8_t board_lowered_limit_switch, uint8_t board_raised_limit_switch)
{
  if (digitalRead(board_lowered_limit_switch) == LOW)
  {
    Serial.println("Board is lowered");
  }
  if (digitalRead(board_raised_limit_switch) == LOW)
  {
    Serial.println("Board is raised");
  }
}


void TestRelays(uint8_t DIR1, uint8_t DIR2, uint8_t Brake_Relay){
  digitalWrite(DIR1, LOW);
  digitalWrite(DIR2, LOW);
  digitalWrite(Brake_Relay, LOW);
  delay(100);

  Serial.println("Testing Direction Relays. DIR1..");
  digitalWrite(DIR1, HIGH);
  delay(10000);
  digitalWrite(DIR1, LOW);

  Serial.println("Testing Change Direction Relay...: DIR2");
  // TOGGLE TURNING TURNING
  digitalWrite(DIR2, HIGH);
  delay(500);
  digitalWrite(DIR2, LOW);
  delay(500);
  digitalWrite(DIR2, LOW);
  delay(500);
  digitalWrite(DIR2, HIGH);
  delay(10000);
  digitalWrite(DIR2, LOW);

  Serial.println("Testing Brake Relay...");
  digitalWrite(Brake_Relay, HIGH);
  delay(1000);
  digitalWrite(Brake_Relay, LOW);
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
