import numpy as np
import json
import time
import joblib
import requests
import paho.mqtt.client as mqtt
from tflite_runtime.interpreter import Interpreter

# ===== LOAD MODEL & SCALER =====
model_temp_mois = Interpreter(model_path="/home/daivjppro/Desktop/temp-mois.tflite")
model_nh3_h2s = Interpreter(model_path="/home/daivjppro/Desktop/nh3-h2s.tflite")
model_temp_mois.allocate_tensors()
model_nh3_h2s.allocate_tensors()

input_details_temp_mois = model_temp_mois.get_input_details()
output_details_temp_mois = model_temp_mois.get_output_details()
input_details_nh3_h2s = model_nh3_h2s.get_input_details()
output_details_nh3_h2s = model_nh3_h2s.get_output_details()

scaler_temp_mois = joblib.load("/home/daivjppro/Desktop/temp_mois_scaler.joblib")
scaler_nh3_h2s = joblib.load("/home/daivjppro/Desktop/nh3_h2s_scaler.joblib")

# ====== MQTT CONFIG ======
def on_message(client, userdata, msg):
    try:
        payload = msg.payload.decode('utf-8')
        data = json.loads(payload)

        temp = float(data["temperature"])
        humid = float(data["humidity"])
        print(f"Received: Temp = {temp}, Humidity = {humid}")

        # --- Scale & Predict ---
        input_temp_mois = scaler_temp_mois.transform([[temp, humid]]).astype(np.float32)
        input_nh3_h2s = scaler_nh3_h2s.transform([[temp, humid]]).astype(np.float32)

        # Thêm chiều thứ ba (time_steps) để dữ liệu có 3 chiều
        input_temp_mois = np.expand_dims(input_temp_mois, axis=1)  # Tạo shape [batch_size, time_steps=1, features]
        input_nh3_h2s = np.expand_dims(input_nh3_h2s, axis=1)  # Tạo shape [batch_size, time_steps=1, features]

        model_temp_mois.set_tensor(input_details_temp_mois[0]['index'], input_temp_mois)
        model_temp_mois.invoke()
        pred_temp_mois = model_temp_mois.get_tensor(output_details_temp_mois[0]['index'])[0]

        model_nh3_h2s.set_tensor(input_details_nh3_h2s[0]['index'], input_nh3_h2s)
        model_nh3_h2s.invoke()
        pred_nh3_h2s = model_nh3_h2s.get_tensor(output_details_nh3_h2s[0]['index'])[0]

        # --- Print Predictions ---
        print(f"Predicted Temp: {pred_temp_mois[0]:.2f}, Humidity: {pred_temp_mois[1]:.2f}")
        print(f"Predicted NH3: {pred_nh3_h2s[0]:.2f}, H2S: {pred_nh3_h2s[1]:.2f}")

        # --- Tính chỉ số THI ---
        thi = 0.8 * temp + humid * (temp - 14.4) + 46.4
        print(f"THI: {thi:.2f}")

        # --- Gửi dữ liệu lên ThingsBoard ---
        tb_payload = {
            "temperature": float(pred_temp_mois[0]),
            "humidity": float(pred_temp_mois[1]),
            "nh3": float(pred_nh3_h2s[0]),
            "h2s": float(pred_nh3_h2s[1])
        }
        tb_url = "http://demo.thingsboard.io/api/v1/JAT8vf6cyR645tnoMYks/telemetry"
        headers = {'Content-Type': 'application/json'}
        try:
            res = requests.post(tb_url, headers=headers, json=tb_payload)
            if res.status_code == 200:
                print("Sent to ThingsBoard successfully.")
            else:
                print("Failed to send to ThingsBoard:", res.status_code)
        except Exception as e:
            print("HTTP error:", e)

        # --- Điều khiển relay qua MQTT cho ESP32 ---
        if thi >= 85.24:
            print("THI cao, bật quạt 15 giây")
            client.publish("esp32/control", "FAN_ON")
            time.sleep(15)
            client.publish("esp32/control", "FAN_OFF")

        if pred_nh3_h2s[0] >= 25 or pred_nh3_h2s[1] >= 10:
            print("NH3 >= 25 hoặc H2S >= 10, bật bơm 15 giây")
            client.publish("esp32/control", "PUMP_ON")
            time.sleep(15)
            client.publish("esp32/control", "PUMP_OFF")
        else:
            client.publish("esp32/control", "FAN_OFF")
            client.publish("esp32/control", "PUMP_OFF")

    except Exception as e:
        print("Error:", e)

# ===== MQTT CLIENT KHỞI TẠO =====
client = mqtt.Client(client_id="PiClient", protocol=mqtt.MQTTv311)
client.on_message = on_message
client.connect("192.168.84.31", 1883, 60)  # IP của Raspberry Pi nếu ESP32 gửi về đây
client.subscribe("esp32/sensors/data")

print("Gateway is listening for sensor data...")
client.loop_forever()