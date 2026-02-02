# Lolin-s2-PZEM004T-MQTT-SSD1306-Dashboard

# ⚡ LOLIN S2 Power Monitor with OLED + MQTT + Button UI

Project ini adalah sistem monitoring daya listrik AC menggunakan **Lolin S2 (ESP32-S2)**, sensor **PZEM-004T**, dan layar **OLED SSD1306**. Sistem dilengkapi dengan antarmuka tombol untuk navigasi data secara lokal dan integrasi ke broker MQTT.

## 🚀 Fitur Utama
* **Real-time Monitoring:** Membaca parameter V, I, P, F, PF, dan kWh setiap 5 detik.
* **8-Page Menu UI:** Navigasi multi-halaman menggunakan tombol fisik pada GPIO 18.
* **Power Saving:** Fitur *Screen Timeout* otomatis mematikan layar setelah 5 menit tanpa interaksi.
* **MQTT Connectivity:** Terhubung ke broker publik untuk sinkronisasi data IoT.

## 🛠️ Komponen Hardware
* **Board:** Lolin S2 (ESP32-S2)
* **Sensor:** PZEM-004T v3.0
* **Display:** OLED 128x64 SSD1306 (I2C)
* **Input:** Push Button (Next Page)

## 📌 Konfigurasi Pinout
| Komponen | Pin Lolin S2 | Fungsi |
| :--- | :--- | :--- |
| **PZEM RX** | GPIO 7 | Hardware Serial 1 |
| **PZEM TX** | GPIO 5 | Hardware Serial 1 |
| **OLED SDA** | GPIO 9 | I2C Data |
| **OLED SCL** | GPIO 11 | I2C Clock |
| **Button** | GPIO 18 | Input Navigasi |

## 🌐 Detail Koneksi
* **MQTT Broker:** `test.mosquitto.org`
* **MQTT Port:** `1883`
* **Client ID:** `LOLIN_S2_PZEM_Node`

## 📟 Navigasi Halaman OLED
1. **Dashboard:** Ringkasan seluruh parameter dalam satu layar.
2. **Voltage:** Detail Tegangan (Volt).
3. **Current:** Detail Arus (Ampere).
4. **Power:** Detail Daya (Watt).
5. **Frequency:** Detail Frekuensi (Hz).
6. **Power Factor:** Detail faktor daya.
7. **Energy:** Detail total penggunaan energi (kWh).
8. **RSSI:** Kekuatan sinyal Wi-Fi (dBm).

## 📝 Catatan Operasional
* **Serial Monitor:** Hanya menampilkan status inisialisasi di awal. Data sensor hanya ditampilkan secara visual melalui layar OLED.
* **Update Interval:** Data diperbarui ke layar setiap 5000ms (5 detik).