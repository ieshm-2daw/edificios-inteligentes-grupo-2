#include <WiFi.h>
#include <PubSubClient.h>

// Configuración de la red Wi-Fi
const char* ssid = "2DAW_IoT";
const char* password = "Somos2DAW";

// Configuración del MQTT
const char* mqtt_server = "ha.ieshm.org";
const int mqtt_port = 1883;
const char* mqtt_client_id = "relay_mqtt";
const char* mqtt_user = "mqtt";      // Reemplaza con el usuario de tu broker MQTT
const char* mqtt_password = "mqtt"; // Reemplaza con la contraseña de tu broker MQTT

const char* mqtt_topic_relay = "esp32/relay";
const char* mqtt_topic_log = "g2/microondas";

WiFiClient espClient;
PubSubClient client(espClient);

const int RelePin = 13;     // Pin del relé
const int RadarPin = 14;    // Pin del sensor RCWL-0516
const int LedPin = 2;       // Pin del LED azul (integrado en la ESP32)

void setup_wifi() {
    delay(10);
    Serial.println("Conectando a WiFi...");
    WiFi.begin(ssid, password);
    WiFi.setTxPower(WIFI_POWER_MINUS_1dBm);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConectado a WiFi, IP: " + WiFi.localIP().toString());
}

// Callback cuando se recibe un mensaje MQTT
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Mensaje recibido [");
    Serial.print(topic);
    Serial.print("]: ");

    String message;
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.println(message);

    if (message == "ON") {
        digitalWrite(RelePin, HIGH);
    } else if (message == "OFF") {
        digitalWrite(RelePin, LOW);
    }
}

// Conectar a MQTT
void reconnect_mqtt() {
    while (!client.connected()) {
        Serial.print("Conectando a MQTT...");
        if (client.connect(mqtt_client_id, mqtt_user, mqtt_password)) {  // 🔹 Se agregaron usuario y contraseña
            Serial.println("conectado.");
            client.subscribe(mqtt_topic_relay);
        } else {
            Serial.print("Error, rc=");
            Serial.print(client.state());
            Serial.println(" Intentando de nuevo en 5 segundos...");
            delay(5000);
        }
    }
}

void setup() {
  // Inicializar los pines
  pinMode(RelePin, OUTPUT);  // Configurar el pin del relé como salida
  pinMode(RadarPin, INPUT);  // Configurar el pin del sensor como entrada
  pinMode(LedPin, OUTPUT);   // Configurar el pin del LED azul como salida

  // Inicializar el relé y el LED
  digitalWrite(RelePin, LOW); // Inicializar el relé apagado
  digitalWrite(LedPin, LOW);  // Inicializar el LED apagado

  // Configurar el Monitor Serie
  Serial.begin(115200); // Iniciar comunicación serie a 115200 baudios
  Serial.println("Sistema iniciado.");

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {


    int valorSensor = digitalRead(RadarPin); // Leer el estado del sensor

    if (valorSensor == HIGH) { // Si detecta movimiento
      digitalWrite(LedPin, HIGH);               // Encender el LED
      Serial.println("Movimiento detectado.");  // Mostrar mensaje en el Monitor Serie

    } else {
      digitalWrite(LedPin, LOW);                // Apagar el LED azul
      Serial.println("No se detectó movimiento."); // Mostrar mensaje en el Monitor Serie

    }
  



}
