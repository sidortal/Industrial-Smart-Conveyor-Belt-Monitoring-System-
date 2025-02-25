#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <HX711.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define LOADCELL_DOUT  4
#define LOADCELL_SCK  5

Adafruit_MPU6050 mpu;
HX711 scale;

// WiFi and MQTT Configuration
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* mqtt_server = "broker.hivemq.com"; 
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  
  // Initialize MPU6050 (Vibration Sensor)
  if (!mpu.begin()) {
    Serial.println("MPU6050 not found!");
    while (1);
  }
  
  // Initialize Load Cell Sensor
  scale.begin(LOADCELL_DOUT, LOADCELL_SCK);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }

  // Connect to MQTT
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    client.connect("ESP32_Conveyor");
  }
  client.loop();

  // Read Vibration Data
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  float vibration = a.acceleration.x;

  // Read Load Cell Data
  float weight = scale.get_units();

  // Create JSON payload
  String payload = "{ \"vibration\": " + String(vibration) +
                   ", \"weight\": " + String(weight) + " }";
  
  // Publish to MQTT topic
  client.publish("conveyor_monitoring/data", payload.c_str());

  Serial.println(payload);
  delay(5000);
}
