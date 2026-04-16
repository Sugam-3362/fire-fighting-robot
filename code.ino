#define FLAME_SENSOR 2
#define RELAY 7
#define TRIG 12
#define ECHO 13
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11
#define ENA 5
#define ENB 6

bool pumpActive = false;

long getDistance() {
  long sum = 0;
  int valid = 0;
  for (int i = 0; i < 3; i++) {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    long duration = pulseIn(ECHO, HIGH, 30000);
    if (duration > 0) {
      sum += duration * 0.034 / 2;
      valid++;
    }
    delay(10);
  }
  if (valid == 0) return 999;
  return sum / valid;
}

void setup() {
  pinMode(FLAME_SENSOR, INPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  digitalWrite(RELAY, HIGH); // pump OFF
  Serial.begin(9600);
}

void forward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, 80);
  analogWrite(ENB, 80);
}

void stopMotor() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void loop() {
  int flame = digitalRead(FLAME_SENSOR);
  long distance = getDistance();
  Serial.print("Flame: "); Serial.print(flame);
  Serial.print(" Distance: "); Serial.println(distance);

  if (flame == LOW) {
    forward();
    if (distance <= 30 && distance != 999 && pumpActive == false) {
      stopMotor();
      pumpActive = true;
      digitalWrite(RELAY, LOW);  // pump ON
      delay(1000);               // spray
      digitalWrite(RELAY, HIGH); // pump OFF
      pumpActive = false;
    }
    return;
  }
  stopMotor();
  digitalWrite(RELAY, HIGH);
  pumpActive = false;
}