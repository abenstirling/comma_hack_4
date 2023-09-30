#include <FlexCAN_T4.h>

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can;

//Motor A - Solenoid (Not used)
const int motorPin1  = 2;  // Pin 14 of L293
const int motorPin2  = 3;  // Pin 10 of L293

//Motor B - Pump (Used)
const int motorPin3  = 4; // Pin  7 of L293
const int motorPin4  = 5;  // Pin  2 of L293

void setup() {
  Serial.begin(9600);
  while (!Serial) {}

  can.begin();
  can.setBaudRate(500000);  // Make sure this baud rate matches your CAN network

  //Set pins as outputs
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
}

void loop() {
  CAN_message_t msg;

  if (can.read(msg)) {
    Serial.print("Received message with ID 0x");
    Serial.print(msg.id, HEX);
    Serial.print(", length ");
    Serial.print(msg.len);
    Serial.print(", data ");

    for (int i = 0; i < msg.len; i++) {
      Serial.print(msg.buf[i], HEX);
      Serial.print(" ");
    }

    Serial.println();

    // Check if this is an ARM_CMD message (ID 0x29A)
    if (msg.id == 0x29A && msg.len >= 2) {
      int8_t arm_vel = msg.buf[0];  // First byte contains the ARM_VEL (signed 8-bit)
      uint8_t arm_suction = (msg.buf[1] & 0x01);  // 9th bit contains the ARM_SUCTION
      uint8_t counter = (msg.buf[1] >> 1) & 0x0F;  // Next 4 bits contain the counter
      uint8_t checksum = msg.buf[1];  // Second byte is the checksum

      Serial.print("ARM_VEL: ");
      Serial.println(arm_vel);
      Serial.print("ARM_SUCTION: ");
      Serial.println(arm_suction);
      Serial.print("COUNTER: ");
      Serial.println(counter);
      Serial.print("CHECKSUM: ");
      Serial.println(checksum);

      // Dictate actions based on arm velocity and arm suction
      if (arm_vel > 0) {
        // Implement code for positive arm velocity
        Serial.println("Positive arm velocity detected. Moving arm up.");
      } else if (arm_vel < 0) {
        // Implement code for negative arm velocity
        Serial.println("Negative arm velocity detected. Moving arm down.");
      } else {
        // Implement code for zero arm velocity
        Serial.println("Zero arm velocity detected. Stopping arm.");
      }

      if (arm_suction == 1) {
        // Implement code for arm suction being ON
        Serial.println("Arm suction is ON. Engaging suction.");
      } else {
        // Implement code for arm suction being OFF
        Serial.println("Arm suction is OFF. Releasing suction.");
      }
    }
  }
}

