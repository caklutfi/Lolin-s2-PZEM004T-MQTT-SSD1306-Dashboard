# ⚡ LOLIN S2 Power Monitor with OLED + MQTT + Button UI

Proyek ini adalah sistem monitoring daya listrik AC menggunakan **Lolin S2 (ESP32-S2)**, sensor **PZEM-004T**, dan layar **OLED SSD1306**. Data dikirimkan secara real-time ke broker MQTT dan ditampilkan pada antarmuka layar lokal.

## 🚀 Fitur Utama
* **Real-time Monitoring:** Pembacaan V, I, P, F, PF, dan kWh setiap 5 detik.
* **MQTT Telemetry:** Pengiriman data ke broker dengan flag *retained* untuk sinkronisasi cepat.
* **8-Page Menu UI:** Navigasi multi-halaman menggunakan satu tombol fisik (GPIO 18).
* **Power Saving:** *Screen Timeout* otomatis mematikan layar setelah 5 menit tanpa interaksi.

## 🌐 Detail Broker & Topik MQTT
Sistem mempublikasikan data ke broker **test.mosquitto.org** dengan struktur topik berikut:

| Parameter | Topik MQTT | Format Data |
| :--- | :--- | :--- |
| **Voltage** | `caklutfi/power/pzem_main/voltage` | Volt (V) |
| **Current** | `caklutfi/power/pzem_main/current` | Ampere (A) |
| **Power** | `caklutfi/power/pzem_main/power` | Watt (W) |
| **Frequency** | `caklutfi/power/pzem_main/frequency` | Hertz (Hz) |
| **Power Factor** | `caklutfi/power/pzem_main/pf` | 0.00 - 1.00 |
| **Energy** | `caklutfi/power/pzem_main/energy` | kiloWatt-hour (kWh) |
| **Uptime** | `caklutfi/power/pzem_main/uptime` | Seconds (s) |

## 📌 Konfigurasi Pinout
| Komponen | Pin Lolin S2 | Fungsi |
| :--- | :--- | :--- |
| **PZEM RX** | GPIO 7 | Hardware Serial 1 |
| **PZEM TX** | GPIO 5 | Hardware Serial 1 |
| **OLED SDA** | GPIO 9 | I2C Data |
| **OLED SCL** | GPIO 11 | I2C Clock |
| **Button** | GPIO 18 | Input Navigasi (Pull-up) |

## 📟 Antarmuka Layar OLED
Gunakan tombol pada **GPIO 18** untuk berpindah antar halaman:
1. **Dashboard:** Ringkasan V, I, P, PF, F, dan E dalam satu layar.
2. **Voltage:** Tampilan besar nilai Tegangan.
3. **Current:** Tampilan besar nilai Arus.
4. **Power:** Tampilan besar nilai Daya.
5. **Frequency:** Tampilan besar nilai Frekuensi.
6. **Power Factor:** Tampilan besar nilai PF.
7. **Energy:** Tampilan besar total kWh sejak alat aktif.
8. **RSSI:** Kekuatan sinyal Wi-Fi dalam dBm.

## 🛠️ Library yang Digunakan
* `PZEM004Tv30` oleh Jakub Mandula
* `PubSubClient` oleh Nick O'Leary
* `Adafruit SSD1306` & `Adafruit GFX`

---
*Catatan: Pastikan konfigurasi WiFi pada kode telah disesuaikan sebelum melakukan flashing.*
