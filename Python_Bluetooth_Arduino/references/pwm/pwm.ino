const int DIR = 10;
const int PUL = 11;

void setup() {
  pinMode(DIR, OUTPUT);
  pinMode(PUL, OUTPUT);

  digitalWrite(DIR, HIGH);
}

void loop() {
  digitalWrite(DIR, !digitalRead(DIR));
  analogWrite(PUL, 255);
  delay(10000);
  analogWrite(PUL, 0);
  delay(10000);
}
