#include <WiFi.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <SoftwareSerial.h>

// ===== WiFi thông tin =====
const char* ssid = "Daivjppro";
const char* password = "123456780";

// ===== MQTT Raspberry Pi =====
const char* mqtt_rpi_host = "192.168.84.31";  // IP của Raspberry Pi
const int mqtt_port = 1883;

// ===== ThingsBoard HTTP Host =====
const char* tb_host = "http://demo.thingsboard.io";

// ===== Access Token của các thiết bị trên ThingsBoard =====
const char* token_dht22   = "tjg1zXDMD6ATSPlr15Cd";
const char* token_nh3     = "zoxBUxHF5ENck2TsS1xQ";
const char* token_h2s     = "ovknIYsHtEXblKHox3jB";
const char* token_gateway = "JAT8vf6cyR645tnoMYks";

// ===== Cảm biến DHT22 =====
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ===== Cảm biến NH3 và H2S =====
SoftwareSerial sensorSerial(16, 17); // RX, TX

// ===== MQTT Client để gửi lên Raspberry Pi =====
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// ===== Kết nối WiFi =====
void setup_wifi() {
  Serial.print("Đang kết nối WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Kết nối WiFi thành công");
}

// ===== Kết nối MQTT đến Raspberry Pi =====
void connect_mqtt() {
  while (!mqttClient.connected()) {
    Serial.print("Kết nối MQTT tới Raspberry Pi...");
    // Dùng access token của gateway để xác thực
    if (mqttClient.connect("ESP32Client", token_gateway, NULL)) {
      Serial.println("Kết nối MQTT thành công");
    } else {
      Serial.print("Thất bại, mã lỗi: ");
      Serial.print(mqttClient.state());
      delay(2000);
    }
  }
}

// ===== Đọc dữ liệu cảm biến NH3 và H2S qua UART =====
String readNH3H2S() {
  String result = "";
  while (sensorSerial.available()) {
    char c = sensorSerial.read();
    result += c;
  }
  result.trim();
  return result;
}

// ===== Gửi dữ liệu lên ThingsBoard qua HTTP =====
void sendToThingsBoard(const char* token, const String& payload) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(tb_host) + "/api/v1/" + token + "/telemetry";

    http.begin(url);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      Serial.print("Đã gửi lên ThingsBoard (token: ");
      Serial.print(token);
      Serial.print(") mã phản hồi: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Gửi thất bại: ");
      Serial.println(http.errorToString(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("Mất kết nối WiFi.");
  }
}

// ===== Setup =====
void setup() {
  Serial.begin(115200);
  dht.begin();
  sensorSerial.begin(9600);

  setup_wifi();  // Kết nối WiFi
  mqttClient.setServer(mqtt_rpi_host, mqtt_port);  // Cấu hình MQTT Raspberry Pi
}

// ===== Loop =====
void loop() {
  if (!mqttClient.connected()) {
    connect_mqtt();  // Kết nối lại nếu mất
  }
  mqttClient.loop();  // Cập nhật vòng lặp MQTT

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  String nh3h2s_data = readNH3H2S();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Lỗi đọc DHT22");
    delay(5000);
    return;
  }

  Serial.print("Nhiệt độ: ");
  Serial.print(temp);
  Serial.print(" °C, Độ ẩm: ");
  Serial.println(hum);
  Serial.print("NH3 và H2S: ");
  Serial.println(nh3h2s_data);

  // ===== Payload gửi lên Raspberry Pi (MQTT) =====
  String payload_rpi = "{";
  payload_rpi += "\"temperature\":" + String(temp) + ",";
  payload_rpi += "\"humidity\":" + String(hum) + ",";
  payload_rpi += "\"nh3_h2s\":\"" + nh3h2s_data + "\"";
  payload_rpi += "}";

  mqttClient.publish("esp32/sensors/data", payload_rpi.c_str());
  Serial.println("Gửi dữ liệu lên Raspberry Pi");

  // ===== Payload gửi lên ThingsBoard =====
  String payload_dht22   = "{\"temperature\":" + String(temp) + ",\"humidity\":" + String(hum) + "}";
  String payload_nh3     = "{\"nh3_h2s\":\"" + nh3h2s_data + "\"}";
  String payload_gateway = "{\"temperature\":" + String(temp) + ",\"humidity\":" + String(hum) + ",\"nh3_h2s\":\"" + nh3h2s_data + "\"}";

  sendToThingsBoard(token_dht22, payload_dht22);
  sendToThingsBoard(token_nh3, payload_nh3);
  sendToThingsBoard(token_gateway, payload_gateway);

  delay(5000);  // Gửi mỗi 5 giây
}
