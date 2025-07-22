#include <WiFi.h>
#include <PubSubClient.h>

// WiFi thông tin
const char* ssid = "Daivjppro";
const char* password = "123456780";

// MQTT Broker (Raspberry Pi)
const char* mqtt_server = "192.168.84.31";  // IP Raspberry Pi
const int mqtt_port = 1883;

// MQTT Client
WiFiClient espClient;
PubSubClient client(espClient);

// Relay Pin Definitions
#define RELAY_PUMP 5  // GPIO5 - Bơm
#define RELAY_FAN  4  // GPIO4 - Quạt

// MQTT Callback - Nhận lệnh từ Raspberry Pi
void callback(char* topic, byte* payload, unsigned int length) {
  String command = "";
  for (unsigned int i = 0; i < length; i++) {
    command += (char)payload[i];
  }

  Serial.print("Received command: ");
  Serial.println(command);

  if (command == "FAN_ON") {
    digitalWrite(RELAY_FAN, HIGH);  // Bật quạt
  } else if (command == "FAN_OFF") {
    digitalWrite(RELAY_FAN, LOW);   // Tắt quạt
  } else if (command == "PUMP_ON") {
    digitalWrite(RELAY_PUMP, HIGH); // Bật bơm
  } else if (command == "PUMP_OFF") {
    digitalWrite(RELAY_PUMP, LOW);  // Tắt bơm
  }
}

// Kết nối WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to WiFi: ");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

// Kết nối MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println(" connected");
      client.subscribe("esp32/control");  // Subscribe topic điều khiển
    } else {
      Serial.print("Failed with state: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PUMP, OUTPUT);  // Khởi tạo relay bơm
  pinMode(RELAY_FAN, OUTPUT);   // Khởi tạo relay quạt
  digitalWrite(RELAY_PUMP, LOW); // Bắt đầu với relay tắt
  digitalWrite(RELAY_FAN, LOW);  // Bắt đầu với relay tắt

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
