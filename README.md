# ⚡ ESP32-Energy-Meter

Sistem pemantauan energi listrik berbasis **ESP32-S2 (Lolin S2)** dan **PZEM-004T v3.0**. Dirancang untuk efisiensi maksimal dengan manajemen WiFi dinamis.

## 🌟 Fitur Utama
- **Ultra-Efficiency Mode**: CPU berjalan pada **80MHz** untuk meminimalkan panas.
- **Dynamic WiFi**: Konfigurasi via portal web (WiFiManager) dengan SSID `ESP32-Energy-Meter`.
- **Smart Interface**: 
  - 8 halaman navigasi OLED.
  - **Screen Timeout (5 Menit)** untuk mencegah *burn-in* dan menghemat daya.
  - Reset WiFi via **Long Press (5 detik)** pada tombol GPIO 18.
- **Full MQTT Reporting**: Data lengkap dikirim setiap 5 detik ke broker.

## 🔌 Pinout Perangkat
| Komponen | Pin Lolin S2 | Keterangan |
| :--- | :--- | :--- |
| **PZEM RX** | GPIO 7 | Sambungkan ke TX PZEM |
| **PZEM TX** | GPIO 5 | Sambungkan ke RX PZEM |
| **OLED SDA** | GPIO 9 | I2C Data |
| **OLED SCL** | GPIO 11 | I2C Clock |
| **Button** | GPIO 18 | Navigasi & Reset |



## 🚀 Jalur Topik MQTT (Lengkap)
Semua data dipublikasikan ke broker `test.mosquitto.org` dengan topik berikut:

1. `caklutfi/power/pzem_main/voltage` - (V)
2. `caklutfi/power/pzem_main/current` - (A)
3. `caklutfi/power/pzem_main/power` - (W)
4. `caklutfi/power/pzem_main/energy` - (kWh)
5. `caklutfi/power/pzem_main/pf` - (cos φ)
6. `caklutfi/power/pzem_main/frequency` - (Hz)
7. `caklutfi/power/pzem_main/uptime` - (Detik)



## 🛠 Instalasi
1. Pastikan library `WiFiManager`, `PubSubClient`, `PZEM004Tv30`, dan `Adafruit SSD1306` terpasang.
2. Unggah kode ke board Lolin S2.
3. Hubungkan HP ke WiFi `ESP32-Energy-Meter` untuk konfigurasi internet.

---
**Dibuat oleh:** caklutfi
