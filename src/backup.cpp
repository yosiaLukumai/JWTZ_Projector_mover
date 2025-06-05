// #include <Arduino.h>
// // #include <freertos/FreeRTOS.h>
// #include <EEPROM.h>
// #define ScreenOneMoved 18
// #define SPEED_OMETER 25
// #define MAX_SPEED 200
// #define Change_Direction 32
// #define screen_reached_limit 5
// #define Brake_Relay 33
// #define LED_2 2
// #define EEPROM_SIZE 1
// #define MOTOR_DIR_STATE 0 // 0 = forward, 1 = reverse
// #define MOTOR_STATE_ADDR 0

// unsigned long last_interrupt_time = 0;
// unsigned long debounce_delay = 50;
// volatile bool limit_triggered = false;

// void IRAM_ATTR handleLimitInterrupt()
// {
//   unsigned long interrupt_time = millis();
//   if (interrupt_time - last_interrupt_time > debounce_delay)
//   {
//     limit_triggered = true;
//     last_interrupt_time = interrupt_time;
//   }
// }
// enum MotorState
// {
//   FORWARD,
//   REVERSE,
//   STOP
// };

// void setMotorState(MotorState state)
// {
//   switch (state)
//   {
//   case FORWARD:
//     break;
//   case REVERSE:
//     break;
//   case STOP:
//     break;
//   }
// }

// void setup()
// {
//   Serial.begin(9600);
//   EEPROM.begin(EEPROM_SIZE);
//   // Factory settings
//   // if (EEPROM.read(MOTOR_STATE_ADDR) == 0)
//   // {
//   //   EEPROM.write(MOTOR_STATE_ADDR, 1);
//   //   EEPROM.commit();
//   // }
//   // delay(5000);

//   pinMode(Change_Direction, OUTPUT);
//   pinMode(screen_reached_limit, INPUT);
//   pinMode(Brake_Relay, OUTPUT);

//   pinMode(ScreenOneMoved, INPUT);
//   attachInterrupt(digitalPinToInterrupt(ScreenOneMoved), handleLimitInterrupt, FALLING);
//   delay(2000);
//   // digitalWrite(Brake_Relay, LOW);
// }

// void testRelay()
// {
//   digitalWrite(Change_Direction, HIGH);
//   delay(1000);
//   digitalWrite(Change_Direction, LOW);
//   delay(1000);
//   digitalWrite(Brake_Relay, HIGH);
//   delay(1000);
//   digitalWrite(Brake_Relay, LOW);
//   delay(1000);
// }

// void flipMotorDirection()
// {
//   digitalWrite(Change_Direction, HIGH);
//   delay(1500);
//   digitalWrite(Change_Direction, LOW);
//   delay(100);
// }

// void setBrakeState(boolean state)
// {
//   digitalWrite(Brake_Relay, state ? HIGH : LOW);
//   delay(10);
// }

// void raiseSpeed(int max)
// {
//   for (int val = 0; val <= max; val++)
//   {
//     dacWrite(SPEED_OMETER, val);
//     delay(2);
//   }
// }

// // For testing the direction of the motor in the both direction
// void testMotorDirection()
// {
//   setBrakeState(true);
//   dacWrite(SPEED_OMETER, 0);
//   flipMotorDirection();
//   setBrakeState(false);
//   raiseSpeed(97);

//   delay(5000);
//   setBrakeState(true);
//   dacWrite(SPEED_OMETER, 0);
//   flipMotorDirection();
//   raiseSpeed(97);
//   setBrakeState(false);
//   delay(5000);
// }

// void loop()
// {
//   testMotorDirection();
//   // if (limit_triggered)
//   // {
//   //   Serial.println("Limit Triggered");
//   //   delay(2000);
//   //   if (digitalRead(ScreenOneMoved) == LOW)
//   //   {
//   //     for (int i = 0; i < 100; i++)
//   //     {
//   //       delay(500);
//   //       while (digitalRead(screen_reached_limit) == HIGH)
//   //       {
//   //         Serial.print("WAITING..... \n");
//   //       }
//   //       if (digitalRead(screen_reached_limit) == LOW)
//   //       {
//   //         Serial.print("Start moving the motor \n");
//   //         setBrakeState(true);
//   //         dacWrite(SPEED_OMETER, 0);
//   //         flipMotorDirection();
//   //         setBrakeState(false);
//   //         raiseSpeed(97);
//   //         delay(5000);
//   //       }
//   //     }
//   //   }
//   //   else
//   //   {
//   //     limit_triggered = false;
//   //     Serial.println("Limit released");
//   //   }
//   // }

// }

// #include <Arduino.h>
// #include <Preferences.h>

// #define LIMIT_LEFT 18
// #define LIMIT_RIGHT 5
// #define BOARD_LOWERED 4
// #define BOARD_RAISED 2

// #define SPEED_OMETER 25
// #define Change_Direction 32
// #define Brake_Relay 33

// Preferences prefs;

// enum SystemState
// {
//   WAIT_BOARD_LOWERED,
//   MOVE_PROJECTOR_RIGHT,
//   WAIT_BOARD_RAISED,
//   MOVE_PROJECTOR_LEFT
// };

// SystemState state;

// // === Motor control ===
// void setBrakeState(bool on)
// {
//   digitalWrite(Brake_Relay, on ? HIGH : LOW);
// }

// void flipDirection()
// {
//   digitalWrite(Change_Direction, HIGH);
//   delay(1500);
//   digitalWrite(Change_Direction, LOW);
// }

// void raiseSpeed(int max)
// {
//   for (int val = 0; val <= max; val++)
//   {
//     dacWrite(SPEED_OMETER, val);
//     delay(2);
//   }
// }

// void stopMotor()
// {
//   dacWrite(SPEED_OMETER, 0);
//   setBrakeState(true);
// }

// // === State storage ===
// void saveState(SystemState s)
// {
//   prefs.begin("projector", false);
//   prefs.putInt("state", s);
//   prefs.end();
// }

// SystemState loadState()
// {
//   prefs.begin("projector", true);
//   int s = prefs.getInt("state", WAIT_BOARD_LOWERED);
//   prefs.end();
//   return (SystemState)s;
// }

// void setup()
// {
//   Serial.begin(9600);

//   prefs.begin("projector", false);

//   pinMode(LIMIT_LEFT, INPUT);
//   pinMode(LIMIT_RIGHT, INPUT);
//   pinMode(BOARD_LOWERED, INPUT);
//   pinMode(BOARD_RAISED, INPUT);

//   pinMode(Brake_Relay, OUTPUT);
//   pinMode(Change_Direction, OUTPUT);

//   setBrakeState(true);
//   state = loadState();
// }

// void loop()
// {
//   switch (state)
//   {
//   case WAIT_BOARD_LOWERED:
//     if (digitalRead(BOARD_LOWERED) == LOW)
//     {
//       Serial.println("Board Lowered, start projector move RIGHT");
//       setBrakeState(false);
//       raiseSpeed(97);
//       state = MOVE_PROJECTOR_RIGHT;
//       saveState(state);
//     }
//     break;

//   case MOVE_PROJECTOR_RIGHT:
//     if (digitalRead(LIMIT_RIGHT) == LOW)
//     {
//       Serial.println("Reached right limit, waiting board raise");
//       stopMotor();
//       state = WAIT_BOARD_RAISED;
//       saveState(state);
//     }
//     break;

//   case WAIT_BOARD_RAISED:
//     if (digitalRead(BOARD_RAISED) == LOW)
//     {
//       Serial.println("Board raised, returning projector LEFT");
//       flipDirection();
//       setBrakeState(false);
//       raiseSpeed(97);
//       state = MOVE_PROJECTOR_LEFT;
//       saveState(state);
//     }
//     break;

//   case MOVE_PROJECTOR_LEFT:
//     if (digitalRead(LIMIT_LEFT) == LOW)
//     {
//       Serial.println("Projector returned to left. Waiting board to lower.");
//       stopMotor();
//       state = WAIT_BOARD_LOWERED;
//       saveState(state);
//     }
//     break;
//   }

//   delay(100);
// }

// #include <Arduino.h>
// #include <Preferences.h>

// // Pin definitions
// #define LIMIT_LEFT 18
// #define LIMIT_RIGHT 5
// #define BOARD_LOWERED 4
// #define BOARD_RAISED 2

// #define SPEED_OMETER 25
// #define Change_Direction 32
// #define Brake_Relay 33

// Preferences prefs;

// enum SystemState
// {
//   WAIT_INITIAL_TRIGGER,   // New: Wait for board signal on startup
//   WAIT_BOARD_LOWERED,
//   MOVE_PROJECTOR_RIGHT,
//   WAIT_BOARD_RAISED,
//   MOVE_PROJECTOR_LEFT
// };

// SystemState state;

// void setBrakeState(bool on)
// {
//   digitalWrite(Brake_Relay, on ? HIGH : LOW);
// }

// void flipDirection()
// {
//   digitalWrite(Change_Direction, HIGH);
//   delay(1200);
//   digitalWrite(Change_Direction, LOW);
// }

// void raiseSpeed(int max)
// {
//   for (int val = 0; val <= max; val++)
//   {
//     dacWrite(SPEED_OMETER, val);
//     delay(2);
//   }
// }

// void stopMotor()
// {
//   dacWrite(SPEED_OMETER, 0);
//   setBrakeState(true);
// }

// void saveState(SystemState s)
// {
//   prefs.begin("projector", false);
//   prefs.putInt("state", s);
//   prefs.end();
// }

// SystemState loadState()
// {
//   prefs.begin("projector", true);
//   int s = prefs.getInt("state", WAIT_INITIAL_TRIGGER);  // Default to WAIT_INITIAL_TRIGGER
//   prefs.end();
//   return (SystemState)s;
// }

// void setup()
// {
//   Serial.begin(9600);

//   pinMode(LIMIT_LEFT, INPUT);
//   pinMode(LIMIT_RIGHT, INPUT);
//   pinMode(BOARD_LOWERED, INPUT);
//   pinMode(BOARD_RAISED, INPUT);

//   pinMode(Brake_Relay, OUTPUT);
//   pinMode(Change_Direction, OUTPUT);

//   setBrakeState(true);
//   state = loadState();
// }

// void loop()
// {
//   switch (state)
//   {
//   case WAIT_INITIAL_TRIGGER:
//     Serial.println("Waiting for board lowered or raised to begin...");
//     if (digitalRead(BOARD_LOWERED) == LOW)
//     {
//       Serial.println("Board is lowered. Ready to move projector RIGHT.");
//       state = WAIT_BOARD_LOWERED;
//       saveState(state);
//     }
//     else if (digitalRead(BOARD_RAISED) == LOW)
//     {
//       Serial.println("Board is raised. Will return projector LEFT when needed.");
//       state = WAIT_BOARD_RAISED;
//       saveState(state);
//     }
//     break;

//   case WAIT_BOARD_LOWERED:
//     if (digitalRead(BOARD_LOWERED) == LOW)
//     {
//       Serial.println("Board Lowered, start projector move RIGHT");
//       setBrakeState(true);
//       flipDirection();
//       delay(500);
//       raiseSpeed(97);
//       setBrakeState(false);
//       state = MOVE_PROJECTOR_RIGHT;
//       saveState(state);
//     }
//     break;

//   case MOVE_PROJECTOR_RIGHT:
//     if (digitalRead(LIMIT_RIGHT) == LOW)
//     {
//       Serial.println("Reached right limit, waiting for board to raise");
//       stopMotor();
//       state = WAIT_BOARD_RAISED;
//       saveState(state);
//     }
//     break;

//   case WAIT_BOARD_RAISED:
//     if (digitalRead(BOARD_RAISED) == LOW)
//     {
//       Serial.println("Board raised, returning projector LEFT");
//       setBrakeState(true);
//       flipDirection();
//       raiseSpeed(97);
//       setBrakeState(false);
//       state = MOVE_PROJECTOR_LEFT;
//       saveState(state);
//     }
//     break;

//   case MOVE_PROJECTOR_LEFT:
//     if (digitalRead(LIMIT_LEFT) == LOW)
//     {
//       Serial.println("Projector returned to left. Waiting for board to lower.");
//       stopMotor();
//       state = WAIT_BOARD_LOWERED;
//       saveState(state);
//     }
//     break;
//   }

//   delay(100); // Polling delay
// }

// #include <Arduino.h>
// ///testing limit switch
// #define LIMIT_LEFT 18
// #define LIMIT_RIGHT 5
// #define BOARD_LOWERED 4
// #define BOARD_RAISED 2

// void setup()
// {
//   Serial.begin(9600);
//   pinMode(LIMIT_LEFT, INPUT);
//   pinMode(LIMIT_RIGHT, INPUT);
//   pinMode(BOARD_LOWERED, INPUT);
//   pinMode(BOARD_RAISED, INPUT);
// }

// void loop()
// {
//   if (digitalRead(LIMIT_LEFT) == LOW)
//   {
//     Serial.println("Limit reached left");
//   }
//   if (digitalRead(LIMIT_RIGHT) == LOW)
//   {
//     Serial.println("Limit reached right");
//   }
//   if (digitalRead(BOARD_LOWERED) == LOW)
//   {
//     Serial.println("Board Lowered");
//   }
//   if (digitalRead(BOARD_RAISED) == LOW)
//   {
//     Serial.println("Board Raised");
//   }
//   delay(100);
// }

// #include <Arduino.h>
// #include <Preferences.h>

// #define LIMIT_LEFT 18
// #define LIMIT_RIGHT 5
// #define BOARD_LOWERED 4
// #define BOARD_RAISED 2

// #define SPEED_OMETER 25
// #define Change_Direction 32
// #define Brake_Relay 33

// Preferences prefs;

// enum SystemState
// {
//   WAIT_INITIAL_TRIGGER,
//   WAIT_BOARD_LOWERED,
//   MOVE_PROJECTOR_RIGHT,
//   WAIT_BOARD_RAISED,
//   MOVE_PROJECTOR_LEFT
// };

// SystemState state;

// enum MotorDirection
// {
//   DIR_RIGHT,
//   DIR_LEFT,
//   DIR_STOPPED
// };

// void saveDirection(MotorDirection dir)
// {
//   prefs.begin("projector", false);
//   prefs.putInt("direction", dir);
//   prefs.end();
// }

// MotorDirection currentDirection = DIR_STOPPED;

// // === Motor control ===
// void setBrakeState(bool on)
// {
//   digitalWrite(Brake_Relay, on ? HIGH : LOW);
// }

// void flipDirection()
// {
//   Serial.println("Flipping direction...");
//   digitalWrite(Change_Direction, HIGH);
//   delay(1000);
//   digitalWrite(Change_Direction, LOW);
//   delay(100);
// }

// void raiseSpeed(int max)
// {
//   for (int val = 0; val <= max; val++)
//   {
//     dacWrite(SPEED_OMETER, val);
//     delay(10);
//   }
// }

// void stopMotor()
// {
//   dacWrite(SPEED_OMETER, 0);
//   setBrakeState(true);
//   currentDirection = DIR_STOPPED;
//   Serial.println("Motor stopped due to limit or logic");
//   saveDirection(currentDirection);
// }

// // === State storage ===
// void saveState(SystemState s)
// {
//   prefs.begin("projector", false);
//   prefs.putInt("state", s);
//   prefs.end();
// }

// MotorDirection loadDirection()
// {
//   prefs.begin("projector", true);
//   int dir = prefs.getInt("direction", DIR_STOPPED);
//   prefs.end();
//   return (MotorDirection)dir;
// }

// SystemState loadState()
// {
//   prefs.begin("projector", true);
//   int s = prefs.getInt("state", WAIT_INITIAL_TRIGGER);
//   prefs.end();
//   return (SystemState)s;
// }

// // === Limit Tracking Task ===
// void trackLimitsTask(void *parameter)
// {
//   while (true)
//   {
//     if (currentDirection == DIR_RIGHT && digitalRead(LIMIT_RIGHT) == LOW)
//     {
//       Serial.println("[Limit Task] Right limit reached!");
//       stopMotor();
//     }
//     else if (currentDirection == DIR_LEFT && digitalRead(LIMIT_LEFT) == LOW)
//     {
//       Serial.println("[Limit Task] Left limit reached!");
//       stopMotor();
//     }
//     else if (currentDirection == DIR_LEFT && digitalRead(LIMIT_RIGHT) == LOW)
//     {
//       Serial.println("[Limit Task] WRONG direction. Right limit still pressed. Re-flipping.");
//       stopMotor();
//       flipDirection();
//       currentDirection = DIR_LEFT;
//       raiseSpeed(95);
//       setBrakeState(false);
//     }
//     else if (currentDirection == DIR_RIGHT && digitalRead(LIMIT_LEFT) == LOW)
//     {
//       Serial.println("[Limit Task] WRONG direction. Left limit still pressed. Re-flipping.");
//       stopMotor();
//       flipDirection();
//       currentDirection = DIR_RIGHT;
//       raiseSpeed(95);
//       setBrakeState(false);
//     }

//     vTaskDelay(pdMS_TO_TICKS(200));
//   }
// }

// void setup()
// {
//   Serial.begin(9600);

//   pinMode(LIMIT_LEFT, INPUT);
//   pinMode(LIMIT_RIGHT, INPUT);
//   pinMode(BOARD_LOWERED, INPUT);
//   pinMode(BOARD_RAISED, INPUT);

//   pinMode(Brake_Relay, OUTPUT);
//   pinMode(Change_Direction, OUTPUT);
//   setBrakeState(true);
//   state = loadState();

  

//   currentDirection = loadDirection();

//   Serial.println("Startup direction: " + String(currentDirection == DIR_LEFT ? "LEFT" : currentDirection == DIR_RIGHT ? "RIGHT"
//                                                                                                                       : "STOPPED"));

//   if (state == MOVE_PROJECTOR_RIGHT)
//   {
//     if (digitalRead(LIMIT_RIGHT) == LOW)
//     {
//       Serial.println("[Resume] RIGHT limit still pressed. Flipping to go LEFT instead.");
//       flipDirection(); // Go the other way
//       currentDirection = DIR_LEFT;
//     }
//     else
//     {
//       Serial.println("[Resume] Resuming RIGHT movement");
//       currentDirection = DIR_RIGHT;
//     }
//     raiseSpeed(96);
//     setBrakeState(false);
//     saveDirection(currentDirection);
//   }
//   else if (state == MOVE_PROJECTOR_LEFT)
//   {
//     if (digitalRead(LIMIT_LEFT) == LOW)
//     {
//       Serial.println("[Resume] LEFT limit still pressed. Flipping to go RIGHT instead.");
//       flipDirection(); // Go the other way
//       currentDirection = DIR_RIGHT;
//     }
//     else
//     {
//       Serial.println("[Resume] Resuming LEFT movement");
//       currentDirection = DIR_LEFT;
//     }
//     raiseSpeed(96);
//     setBrakeState(false);
//     saveDirection(currentDirection);
//   }
//   else
//   {
//     Serial.println("[Resume] Not moving. Brake ON.");
//     currentDirection = DIR_STOPPED;
//     setBrakeState(true);
//     saveDirection(currentDirection);
//   }

//   xTaskCreate(
//       trackLimitsTask,
//       "TrackLimitsTask",
//       2048,
//       NULL,
//       1,
//       NULL);
// }

// void loop()
// {
//   Serial.println("State: " + String(state));
//   Serial.println("Current Direction: " + String(currentDirection));
//   switch (state)
//   {
//   case WAIT_INITIAL_TRIGGER:
//   Serial.println("Waiting for board lowered or raised to begin...");
//   if (digitalRead(BOARD_LOWERED) == LOW)
//     {
//       Serial.println("Board is lowered. Ready to move projector RIGHT.");
//       state = WAIT_BOARD_LOWERED;
//       saveState(state);
//     }
//     else if (digitalRead(BOARD_RAISED) == LOW)
//     {
//       Serial.println("Board is raised. Will return projector LEFT when needed.");
//       state = WAIT_BOARD_RAISED;
//       saveState(state);
//     }
//     break;

//   case WAIT_BOARD_LOWERED:
//     if (digitalRead(BOARD_LOWERED) == LOW)
//     {
//       Serial.println("Board Lowered, start projector move RIGHT");
//       flipDirection();
//       raiseSpeed(96);
//       setBrakeState(false);
//       currentDirection = DIR_RIGHT;
//       saveDirection(currentDirection);
//       state = MOVE_PROJECTOR_RIGHT;
//       saveState(state);
//     }
//     break;

//   case MOVE_PROJECTOR_RIGHT:
//     if (digitalRead(LIMIT_RIGHT) == LOW)
//     {
//       Serial.println("Reached right limit, waiting for board to raise");
//       stopMotor();
//       state = WAIT_BOARD_RAISED;
//       saveState(state);
//     }
//     break;

//   case WAIT_BOARD_RAISED:
//     if (digitalRead(BOARD_RAISED) == LOW)
//     {
//       Serial.println("Board raised, returning projector LEFT");
//       flipDirection();
//       raiseSpeed(96);
//       setBrakeState(false);
//       currentDirection = DIR_LEFT;
//       saveDirection(currentDirection);

//       state = MOVE_PROJECTOR_LEFT;
//       saveState(state);
//     }
//     break;

//   case MOVE_PROJECTOR_LEFT:
//     if (digitalRead(LIMIT_LEFT) == LOW)
//     {
//       Serial.println("Projector returned to left. Waiting for board to lower.");
//       stopMotor();
//       state = WAIT_BOARD_LOWERED;
//       saveState(state);
//     }
//     break;
//   }
//   delay(100);
// }

// #include <Arduino.h>

// #define SPEED_OMETER 25
// #define Change_Direction 32
// #define Brake_Relay 33

// // Optional: limit switches for safety
// #define LIMIT_LEFT 18
// #define LIMIT_RIGHT 5

// enum MotorDirection
// {
//   DIR_RIGHT,
//   DIR_LEFT,
//   DIR_STOPPED
// };

// MotorDirection currentDirection = DIR_STOPPED;

// void setBrakeState(bool on)
// {
//   digitalWrite(Brake_Relay, on ? HIGH : LOW);
// }

// void raiseSpeed(int max)
// {
//   for (int val = 0; val <= max; val++)
//   {
//     dacWrite(SPEED_OMETER, val);
//     delay(5); // faster ramp up for test
//   }
// }

// void stopMotor()
// {
//   dacWrite(SPEED_OMETER, 0);
//   setBrakeState(true);
//   currentDirection = DIR_STOPPED;
//   Serial.println("Motor stopped");
// }

// void flipDirection()
// {
//   Serial.println("Flipping direction...");
//   setBrakeState(true); // brake before flipping
//   digitalWrite(Change_Direction, HIGH);
//   delay(1000);
//   digitalWrite(Change_Direction, LOW);
//   delay(100);

//   // Update direction logic
//   if (currentDirection == DIR_LEFT)
//     currentDirection = DIR_RIGHT;
//   else if (currentDirection == DIR_RIGHT)
//     currentDirection = DIR_LEFT;

//   Serial.println("New direction: " + String(currentDirection == DIR_LEFT ? "LEFT" : "RIGHT"));
// }

// void setup()
// {
//   Serial.begin(9600);

//   pinMode(Brake_Relay, OUTPUT);
//   pinMode(Change_Direction, OUTPUT);

//   pinMode(LIMIT_LEFT, INPUT);
//   pinMode(LIMIT_RIGHT, INPUT);

//   setBrakeState(true);
//   delay(1000); // Wait before starting

//   Serial.println("Starting at center. Moving RIGHT...");
//   currentDirection = DIR_LEFT;
//   flipDirection();  // Ensure it's facing right
//   setBrakeState(false);
//   raiseSpeed(96);
//   delay(1000); // run briefly
//   stopMotor();

//   delay(2000); // pause at center

//   Serial.println("Now moving LEFT...");
//   flipDirection();  // Switch to left
//   setBrakeState(false);
//   raiseSpeed(96);
//   delay(1000); // run briefly
//   stopMotor();

//   Serial.println("Test complete.");
// }

// void loop()
// {
//   // Do nothing. One-time test.
// }
