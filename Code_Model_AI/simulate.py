import paho.mqtt.client as mqtt
import time
import random
import threading

# MQTT Broker
mqtt_server = "192.168.84.31"  # Địa chỉ IP của Raspberry Pi
mqtt_port = 1883
mqtt_topic = "esp32/control"

# Kết nối MQTT
client = mqtt.Client(client_id="RaspberryPi")
client.connect(mqtt_server, mqtt_port, 60)

# Giả lập các giá trị cảm biến
def simulate_sensor_data():
    # Giả lập nhiệt độ (từ 25°C đến 35°C)
    temp = random.uniform(25.0, 35.0)
    
    # Giả lập độ ẩm (từ 40% đến 90%)
    humidity = random.uniform(40.0, 90.0)
    
    # Giả lập nồng độ NH3 (từ 10 đến 30 ppm)
    nh3 = random.uniform(10, 30)
    
    # Giả lập nồng độ H2S (từ 5 đến 20 ppm)
    h2s = random.uniform(5, 20)
    
    # Tính chỉ số THI
    thi = 0.8 * temp + humidity * (temp - 14.4) + 46.4
    
    return temp, humidity, nh3, h2s, thi

# Hàm gửi tín hiệu MQTT
def send_control_signal(signal):
    payload = signal
    client.publish(mqtt_topic, payload)
    print(f"Sent signal: {signal}")

# Bật relay và tắt sau 15 giây
def control_relay_with_timer(relay_type):
    send_control_signal(f"{relay_type}_ON")
    time.sleep(15)
    send_control_signal(f"{relay_type}_OFF")
    print(f"{relay_type} relay turned off after 15 seconds.")

# Giả lập hoạt động và kiểm tra ngưỡng
def check_thresholds():
    while True:
        temp, humidity, nh3, h2s, thi = simulate_sensor_data()
        
        print(f"Simulated Data - Temp: {temp:.2f}°C, Humidity: {humidity:.2f}%, NH3: {nh3:.2f}ppm, H2S: {h2s:.2f}ppm, THI: {thi:.2f}")
        
        # Kiểm tra ngưỡng và gửi tín hiệu điều khiển
        if thi >= 85.24:
            print("THI >= 85.24, turning on fan relay for 15 seconds.")
            threading.Thread(target=control_relay_with_timer, args=("FAN",)).start()  # Bật quạt
        if nh3 >= 25:
            print("NH3 >= 25ppm, turning on pump relay for 15 seconds.")
            threading.Thread(target=control_relay_with_timer, args=("PUMP",)).start()  # Bật bơm
        if h2s >= 10:
            print("H2S >= 10ppm, turning on pump relay for 15 seconds.")
            threading.Thread(target=control_relay_with_timer, args=("PUMP",)).start()  # Bật bơm

        time.sleep(5)  # Mỗi 5 giây gửi tín hiệu một lần

# Kết nối MQTT và chạy kiểm tra ngưỡng
client.loop_start()  # Bắt đầu vòng lặp MQTT
check_thresholds()  # Kiểm tra các ngưỡng và gửi tín hiệu
