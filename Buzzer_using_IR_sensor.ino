int IR = 8;
int BZ = 11;

void setup() {
  // put your setup code here, to run once:
pinMode(IR, INPUT);
pinMode(BZ, OUTPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
int status = digitalRead(IR);

if(status == LOW)
{
  digitalWrite(BZ, HIGH);
  Serial.println("HIGH");
  delay(1000);
  }
else if (status == HIGH)
{
  digitalWrite(BZ, LOW);
  delay(1000);
  Serial.println("LOW");
  }
}
