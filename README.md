# ⚡ Lolin S2 PZEM-004T Power Monitor

Monitor daya listrik cerdas berbasis **ESP32-S2 (Lolin S2)** yang dirancang untuk efisiensi suhu tinggi dan kemudahan penggunaan. Perangkat ini mengirimkan data sensor listrik secara real-time ke broker MQTT dan memiliki antarmuka lokal pada layar OLED.

## ✨ Fitur Unggulan
- **Ultra-Efficiency Mode**: Berjalan pada clock **80MHz** dengan **WiFi Modem Sleep** untuk menjaga suhu MCU tetap dingin meskipun beroperasi 24/7.
- **Dynamic WiFi Configuration**: Menggunakan **WiFiManager**, sehingga SSID/Password WiFi dapat diatur melalui HP tanpa perlu upload ulang kode.
- **Dual-Function Button**: 
    - *Short Press*: Berpindah antar 8 halaman tampilan OLED.
    - *Long Press (5 detik)*: Reset pengaturan WiFi dan kembali ke mode portal.
- **Full MQTT Integration**: Publikasi data lengkap (V, I, P, E, PF, Freq, Uptime) untuk dashboard seperti Home Assistant atau Node-RED.
- **8-Page OLED UI**: Dashboard ringkas dan tampilan detail per parameter dengan font besar.

## 🔌 Diagram Pinout (Lolin S2)

| Komponen | Pin Lolin S2 | Fungsi |
| :--- | :--- | :--- |
| **PZEM-004T** | GPIO 7 (RX), GPIO 5 (TX) | Komunikasi Serial Sensor |
| **OLED SSD1306** | GPIO 9 (SDA), GPIO 11 (SCL) | Antarmuka I2C |
| **Push Button** | GPIO 18 | Navigasi & Reset WiFi |



## 🛠 Persiapan & Instalasi

### 1. Library yang Dibutuhkan
Pastikan Anda telah menginstal library berikut melalui Library Manager di Arduino IDE:
- `PZEM004Tv30` oleh Jakub Mandel
- `PubSubClient` oleh Nick O'Leary
- `WiFiManager` oleh tzapu
- `Adafruit SSD1306` & `Adafruit GFX Library`

### 2. Cara Penggunaan
1. **Pemasangan**: Hubungkan PZEM-004T ke beban AC dan ESP32 sesuai pinout di atas.
2. **Koneksi WiFi**:
   - Saat pertama kali menyala, cari WiFi bernama `ESP32_Power_Monitor` menggunakan HP Anda.
   - Buka browser (biasanya otomatis muncul), pilih WiFi rumah Anda, dan masukkan password.
3. **Monitoring**:
   - Data akan otomatis muncul di layar OLED.
   - Pantau data jarak jauh menggunakan **MQTT Explorer** atau dashboard favorit Anda.

## 📡 Topik MQTT
Data dipublikasikan setiap 5 detik ke broker `test.mosquitto.org` (atau broker pilihan Anda) dengan format berikut:

| Parameter | Topik MQTT | Satuan |
| :--- | :--- | :--- |
| **Voltage** | `caklutfi/power/pzem_main/voltage` | Volt (V) |
| **Current** | `caklutfi/power/pzem_main/current` | Ampere (A) |
| **Power** | `caklutfi/power/pzem_main/power` | Watt (W) |
| **Energy** | `caklutfi/power/pzem_main/energy` | kWh |
| **Frequency** | `caklutfi/power/pzem_main/frequency` | Hz |
| **Power Factor** | `caklutfi/power/pzem_main/pf` | cos φ |
| **Uptime** | `caklutfi/power/pzem_main/uptime` | Detik |



## ⚠️ Catatan Teknis
- **Suhu MCU**: Perangkat ini telah dioptimasi untuk berjalan di 80MHz. Jika Anda mengubah kode, perhatikan urutan inisialisasi I2C agar layar tidak blank.
- **Keamanan**: Selalu berhati-hati saat bekerja dengan tegangan tinggi AC 220V pada modul PZEM-004T. Pastikan isolasi kabel terpasang dengan baik.

---
**Author**: caklutfi  
**Hardware**: Lolin S2 (ESP32-S2), PZEM-004T v3.0, OLED SSD1306 128x64.
