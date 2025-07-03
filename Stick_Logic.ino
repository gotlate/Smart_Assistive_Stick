#define trigPin1 7   //right
#define echoPin1 8   //right

#define trigPin2 3   //left
#define echoPin2 2  //left

#define trigPin3 10   //front-bottom
#define echoPin3 9     //front-bottom

#define trigPin4 5  //middle-front
#define echoPin4 6  //middle-front

#define buzzerPin 12

#define gasPin A5
#define gasThreshold 100

#define waterPin A3
#define waterThreshold 50

#define tiltPin 4

#define firePin A0
#define fireThreshold 650

enum AlertType {
  NONE,
  OBSTACLE_LEFT,
  OBSTACLE_RIGHT,
  OBSTACLE_FRONT,
  WATER,
  GAS,
  TILT,
  FLAME
};

long readDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2;  // in cm
  return distance;
}

void shortBeep() {
  digitalWrite(buzzerPin, LOW);  // LOW trigger buzzer
  delay(200);
  digitalWrite(buzzerPin, HIGH);
}

void doubleBeep() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(buzzerPin, LOW);
    delay(100);
    digitalWrite(buzzerPin, HIGH);
    delay(100);
  }
}

void longBeep() {
  digitalWrite(buzzerPin, LOW);
  delay(600);
  digitalWrite(buzzerPin, HIGH);
}

void gasdetected() {
  for (int i = 0; i < 2; i++) {  // Repeat the pattern twice
    for (int j = 0; j < 3; j++) {  // Three quick beeps
      digitalWrite(buzzerPin, LOW);
      delay(100);
      digitalWrite(buzzerPin, HIGH);
      delay(100);
    }
    delay(300);  // Slight break between sets
  }
}

void waterDetected() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(buzzerPin, LOW);
    delay(200);
    digitalWrite(buzzerPin, HIGH);
    delay(200);
  }
  delay(600);  // Long pause to feel like a wave
}

void tiltAlert() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(buzzerPin, LOW);
    delay(100);
    digitalWrite(buzzerPin, HIGH);
    delay(100);
  }
  delay(200);

  for (int i = 0; i < 2; i++) {
    digitalWrite(buzzerPin, LOW);
    delay(100);
    digitalWrite(buzzerPin, HIGH);
    delay(100);
  }
}

void flameAlert() {
  // Long beep
  digitalWrite(buzzerPin, LOW);
  delay(500);
  digitalWrite(buzzerPin, HIGH);
  delay(200);

  // Short beep
  digitalWrite(buzzerPin, LOW);
  delay(200);
  digitalWrite(buzzerPin, HIGH);
  delay(200);

  // Long beep
  digitalWrite(buzzerPin, LOW);
  delay(500);
  digitalWrite(buzzerPin, HIGH);
  delay(400);
}


void setup() {
  delay(60000);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);

  pinMode(trigPin4, OUTPUT);
  pinMode(echoPin4, INPUT);

  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, HIGH);  // Turn off buzzer initially

  pinMode(gasPin, INPUT);

  pinMode(waterPin, INPUT);

  pinMode(tiltPin, INPUT);

  pinMode(firePin, INPUT);

  Serial.begin(9600);

}

void loop() {
  
  long distRight = readDistance(trigPin1, echoPin1);
  long distLeft = readDistance(trigPin2, echoPin2);
  long distFront1 = readDistance(trigPin3, echoPin3);
  long distFront2 = readDistance(trigPin4, echoPin4);

  int gasValue = analogRead(gasPin);
  int waterValue = analogRead(waterPin);
  int tiltState = digitalRead(tiltPin);
  int flameValue = analogRead(firePin);

  Serial.print("Tilt State: "); Serial.print(tiltState);
  Serial.print(" Bottom-Front: "); Serial.print(distFront1);
  Serial.print(" cm | Middle-Front: "); Serial.print(distFront2);
  Serial.print(" cm | Left: "); Serial.print(distLeft);
  Serial.print(" cm | Right: "); Serial.print(distRight);
  Serial.print(" cm | Gas: "); Serial.print(gasValue);
  Serial.print(" | Water: "); Serial.print(waterValue);
  Serial.print(" | Flame: "); Serial.print(flameValue);

  AlertType currentAlert = NONE;
  static bool isHandlingObstacle = false;
  static bool waterAlertActive = false;
  static unsigned long tiltLowStartTime = 0;
  static bool tiltAlertActive = false;


  // Priority-based event decision
  if (gasValue > gasThreshold) {
    currentAlert = GAS;
  } 
  if (flameValue < fireThreshold && currentAlert == NONE) {
    currentAlert = FLAME;
  }
  if (waterValue > waterThreshold && !waterAlertActive && currentAlert == NONE) {
    currentAlert = WATER;
    waterAlertActive = true;
  }
  else if(waterValue <= waterThreshold) {
    waterAlertActive = false;
  }  
// Tilt logic: alert if sensor is HIGH (tilted)
// or has been LOW (vertical) for less than 1 second (unstable handling)
  if (tiltState == LOW) {
    // Start or continue timing vertical state
    if (tiltLowStartTime == 0) {
      tiltLowStartTime = millis();
    }

    // If it’s LOW for over 1 second, reset alert
    if ((millis() - tiltLowStartTime) >= 1000) {
      tiltAlertActive = false;  // stable
    } 
    else {
      tiltAlertActive = true;   // unstable (too quick)
    }
  } 
  else {
    // HIGH = tilted, so immediate alert
    tiltLowStartTime = 0;
    tiltAlertActive = true;
  }

  if (tiltAlertActive && currentAlert == NONE) {
    currentAlert = TILT;
  }

  if (!isHandlingObstacle && (distFront1 < 35 || distFront2 < 35) && currentAlert == NONE) {
    currentAlert = OBSTACLE_FRONT;
    isHandlingObstacle = true;
  }
  else {
    isHandlingObstacle = false;
  }
Serial.print(" Current Alert: "); Serial.println(currentAlert);

  // Handle the alert
  switch (currentAlert) {
    case GAS:
      gasdetected();
      break;

    case FLAME:
      flameAlert();
      break;

    case WATER:
      waterDetected();
      break;

    case TILT:
      tiltAlert();
      break;

    case OBSTACLE_FRONT:
      longBeep();
      delay(300);
      // Directional decision logic
      if (distRight > distLeft) {
        shortBeep();
        delay(100);
      } else {
        doubleBeep();
        delay(100);
      }
      break;

    case NONE:
    default:
      // Do nothing
      break;
  }
  delay(500);
}





