void setup()
{
  pinMode(18, OUTPUT); // Configurar el pin del LED incorporado como salida
}

void loop()
{
  Serial.println("El programa está corriendo...");
  digitalWrite(18, HIGH); // Encender el LED
  delay(1000); // Esperar un segundo
  digitalWrite(18, LOW); // Apagar el LED
  delay(1000); // Esperar un segundo
}