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



