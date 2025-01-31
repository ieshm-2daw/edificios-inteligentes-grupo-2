#include <BH1750.h>
#include <Wire.h>

BH1750 sensor;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  sensor.begin();
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned int lux = sensor.readLightLevel();
  Serial.print("Nivel: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(1000);
}