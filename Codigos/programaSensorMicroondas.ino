#include <WiFi.h>
#include <PubSubClient.h>

// Configuración de la red Wi-Fi
const char* ssid = "ssid WIFI";
const char* password = "contraseña WIFI";

// Configuración del MQTT
const char* mqtt_server = "dominio del MQTT";
const int mqtt_port = "puerto (xxxx)";
const char* mqtt_client_id = "id sensor Mqtt";
const char* mqtt_user = "usuario mqtt";
const char* mqtt_password = "password mqtt";

const char* mqtt_topic_log = "g2/microondas";

WiFiClient espClient;
PubSubClient client(espClient);

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

// Conectar a MQTT
void reconnect_mqtt() {
    while (!client.connected()) {
        Serial.print("Conectando a MQTT...");
        if (client.connect(mqtt_client_id, mqtt_user, mqtt_password)) {
            Serial.println("conectado.");
        } else {
            Serial.print("Error, rc=");
            Serial.print(client.state());
            Serial.println(" Intentando de nuevo en 5 segundos...");
            delay(5000);
        }
    }
}

void setup() {
    pinMode(RadarPin, INPUT);  // Configurar el pin del sensor como entrada
    pinMode(LedPin, OUTPUT);   // Configurar el pin del LED azul como salida

    digitalWrite(LedPin, LOW);  // Inicializar el LED apagado

    Serial.begin(115200);
    Serial.println("Sistema iniciado.");
    setup_wifi();
    client.setServer(mqtt_server, mqtt_port);
    
}

void loop() {
    if (!client.connected()) {
        reconnect_mqtt();
    }
    client.loop();

    int valorSensor = digitalRead(RadarPin);

    if (valorSensor == HIGH) {
        digitalWrite(LedPin, HIGH);
        Serial.println("Movimiento detectado.");
        client.publish(mqtt_topic_log, "Movimiento detectado");
    } else {
        digitalWrite(LedPin, LOW);
        Serial.println("No se detectó movimiento.");
        client.publish(mqtt_topic_log, "No se detectó movimiento");
    }

    delay(3600);
}