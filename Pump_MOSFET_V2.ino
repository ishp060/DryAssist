int PWMA = 3;
int TOn = 5000;
int TOff = A0;
float OffMax = 60000;


void setup() {
  pinMode(PWMA,OUTPUT);
  pinMode(TOff, INPUT);
  Serial.begin(9600);

}

void loop() {
  int TOffRaw = analogRead(TOff);
  Serial.println(TOffRaw);
  float TOff = TOffRaw/1023*OffMax;
  Serial.println(TOff);
  int TOffPrint = (TOff/1000);
  Serial.println(" sec");
  Serial.print("Pump is off for ");
  Serial.print(TOffPrint);
  Serial.println(" sec");
  analogWrite(PWMA,1023);
  delay(TOn);
  analogWrite(PWMA,LOW);
  delay(TOff);
}

