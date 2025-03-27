int PWMA = 3;
float TOn = 5000;
float TOff = 5000;
float Voltage = 12;


void setup() {
  pinMode(PWMA,OUTPUT);
  Serial.begin(9600);

}

void loop() {
  Serial.print("Pump is on for ");
  int TOnPrint = (TOn/1000);
  int TOffPrint = (TOff/1000);
  Serial.print(TOnPrint);
  Serial.println(" sec");
  Serial.print("Pump is off for ");
  Serial.print(TOffPrint);
  Serial.println(" sec");
  analogWrite(PWMA, 1023);
  delay(TOn);
  analogWrite(PWMA,LOW);
  delay(TOff);
}

