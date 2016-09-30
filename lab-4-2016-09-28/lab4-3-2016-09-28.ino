float valueX, valueY;
int restX, restY;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  restX = analogRead(0);
  restY = analogRead(1);
}

void loop() {
  int val0 = analogRead(0);
  int val1 = analogRead(1);

  valueX = (val0 - restX)/67.584;
  valueY = (val1 - restY)/67.584;

  Serial.print("X:");
  Serial.print(valueX);
  Serial.print(";"); // splitter character for indexOf() in Processing
  Serial.print("Y:");
  Serial.print(valueY);

  Serial.println("");

  delay(30);
}
