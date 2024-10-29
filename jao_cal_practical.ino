#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

// Pin definitions
#define RST_PIN         9
#define SS_PIN          10
#define SERVO_PIN       2
#define TRIG_PIN        4
#define ECHO_PIN        5
#define LED_GREEN       6
#define LED_RED         7
#define LED_PROXIMITY   8    // New LED for proximity detection

// Constants
#define LOCK_POSITION   0
#define UNLOCK_POSITION 90
#define PROXIMITY_THRESHOLD 30  // Distance in cm to trigger proximity detection
#define LOCK_DELAY     5000    // Time in ms before auto-locking
#define CARD_LENGTH    4       // Length of RFID card UID

// Initialize objects
MFRC522 rfid(SS_PIN, RST_PIN);
Servo lockServo;


// Variables
bool isLocked = true;
bool proximityDetected = false;
unsigned long lastUnlockTime = 0;
unsigned long lastProximityTime = 0;
const unsigned long PROXIMITY_LED_TIMEOUT = 1000; // LED timeout in ms

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Initialize SPI bus
  SPI.begin();
  
  // Initialize RFID module
  rfid.PCD_Init();
  
  // Initialize servo
  lockServo.attach(SERVO_PIN);
  lockServo.write(LOCK_POSITION);
  
  // Initialize ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Initialize LED pins
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_PROXIMITY, OUTPUT);  // Initialize proximity LED
  
  // Initial LED state
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_PROXIMITY, LOW);
  
  Serial.println("Smart Lock System Initialized");
}

void loop() {
  // Check proximity
  float distance = getDistance();
  
  // Update proximity detection status and LED
  if (distance < PROXIMITY_THRESHOLD) {
    if (!proximityDetected) {
      Serial.println("Motion detected! Distance: " + String(distance) + " cm");
    }
    proximityDetected = true;
    lastProximityTime = millis();
    digitalWrite(LED_PROXIMITY, HIGH);
    
    // Check RFID when someone is near
    checkRFID();
  } else {
    // Turn off proximity LED after timeout
    if (proximityDetected && (millis() - lastProximityTime > PROXIMITY_LED_TIMEOUT)) {
      proximityDetected = false;
      digitalWrite(LED_PROXIMITY, LOW);
    }
  }
  
  // Auto-lock after delay
  if (!isLocked && (millis() - lastUnlockTime > LOCK_DELAY)) {
    lock();
  }
}

float getDistance() {
  // Clear the TRIG_PIN
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // Send ultrasonic pulse
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Read the ECHO_PIN
  long duration = pulseIn(ECHO_PIN, HIGH);
  
  // Calculate distance in cm
  return duration * 0.034 / 2;
}

void checkRFID() {
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) { // NUID has been readed
    Serial.print("UID:");
    String TagID = "";
    for(byte i = 0; i < rfid.uid.size; i++){
      TagID += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
      TagID += String(rfid.uid.uidByte[i], HEX);
      TagID.toUpperCase();
    }
    Serial.println(TagID);
    
    if(TagID == "A3DE32AD"){
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_PROXIMITY, LOW);
      unlock();
    }
  }
}

void unlock() {
  Serial.println("Unlocking...");
  lockServo.write(UNLOCK_POSITION);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);
  isLocked = false;
  lastUnlockTime = millis();
}

void lock() {
  Serial.println("Locking...");
  lockServo.write(LOCK_POSITION);
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW);
  isLocked = true;
}
