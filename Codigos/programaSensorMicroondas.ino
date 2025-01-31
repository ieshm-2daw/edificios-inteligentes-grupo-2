const int LEDPin = 2;
const int RadarPin = 18;
 
void setup()
{
  pinMode(LEDPin, OUTPUT);
  pinMode(RadarPin, INPUT);
}
 
void loop()
{
  int value= digitalRead(RadarPin);
 
  if (value == HIGH)
  {
    digitalWrite(LEDPin, HIGH);
    delay(500);
    digitalWrite(LEDPin, LOW);
    delay(500);
  }
  else
  {
    digitalWrite(LEDPin, LOW);
  }
}