/**************************************************************
   LOLIN S2 Power Monitor - Ultra Efficiency (80MHz)
   WiFiManager + Full MQTT (V, I, P, E, PF, Freq)
**************************************************************/

#include <WiFi.h>
#include <WebServer.h>
#include <WiFiManager.h> 
#include <PubSubClient.h>
#include <PZEM004Tv30.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ================= KONFIGURASI MQTT =================
#define MQTT_SERVER   "test.mosquitto.org"
#define MQTT_PORT     1883

WiFiClient espClient;
PubSubClient client(espClient);

HardwareSerial pzemSerial(1);
#define PZEM_RX 7
#define PZEM_TX 5
PZEM004Tv30 pzem(pzemSerial, PZEM_RX, PZEM_TX);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ================= TOMBOL & TIMING =================
#define BTN_PIN 18
unsigned long buttonPressedTime = 0;
const int LONG_PRESS_TIME = 5000; 
bool isPressing = false;

unsigned long lastSensorRead = 0;
const unsigned long sensorInterval = 5000;
unsigned long lastMQTTRetry = 0;
unsigned long startTimeMillis = 0;

int displayPage = 0;
const int totalPages = 8;
float energyStart = 0;
float voltage, current, power, frequency, pf, energyKWh;

// ================= HELPER FUNCTIONS =================
String getDurationAgo() {
  unsigned long totalSeconds = (millis() - startTimeMillis) / 1000;
  int hours = totalSeconds / 3600;
  int minutes = (totalSeconds % 3600) / 60;
  int seconds = totalSeconds % 60;
  char buf[20];
  sprintf(buf, "%02d:%02d:%02d ago", hours, minutes, seconds);
  return String(buf);
}

void showMessage(const char* msg) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,20);
  display.println(msg);
  display.display();
}

void setupWiFiManager() {
  WiFiManager wm;
  showMessage("Portal Active:\nESP32_Power_Monitor");
  if (!wm.autoConnect("ESP32_Power_Monitor")) {
    ESP.restart();
  }
  WiFi.setSleep(true); 
}

void connectMQTT() {
  if (!client.connected() && (millis() - lastMQTTRetry > 5000)) {
    lastMQTTRetry = millis();
    if (client.connect("LOLIN_S2_PZEM_Node")) {
      Serial.println("[MQTT] Connected!");
    }
  }
}

// ================= UI FUNCTIONS =================
void drawDashboard() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.drawRect(0, 0, 64, 16, SSD1306_WHITE);
  display.drawRect(64, 0, 64, 16, SSD1306_WHITE);
  display.setCursor(4,4);   display.printf("V %.0fV", voltage);
  display.setCursor(68,4);  display.printf("I %.2fA", current);
  display.drawRect(0, 16, 64, 16, SSD1306_WHITE);
  display.drawRect(64, 16, 64, 16, SSD1306_WHITE);
  display.setCursor(4,20);  display.printf("P %.0fW", power);
  display.setCursor(68,20); display.printf("PF %.2f", pf);
  display.drawRect(0, 32, 64, 16, SSD1306_WHITE);
  display.drawRect(64, 32, 64, 16, SSD1306_WHITE);
  display.setCursor(4,36);  display.printf("F %.1fHz", frequency);
  display.setCursor(68,36); display.printf("E %.3fkWh", energyKWh);
  display.setCursor(0,56);
  display.print(WiFi.localIP());
  display.display();
}

void drawBig(const char* label, float value, const char* unit, bool noDecimal=false) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println(label);
  display.setTextSize(2);
  display.setCursor(0,20);
  if (noDecimal) display.printf("%.0f %s", value, unit);
  else display.printf("%.2f %s", value, unit);
  display.setTextSize(1);
  display.setCursor(0,56);
  display.print(WiFi.localIP());
  if (displayPage == 6) { 
    display.setCursor(0, 42);
    display.print("since: "); display.print(getDurationAgo());
  }
  display.display();
}

void drawCurrentPage() {
  switch (displayPage) {
    case 0: drawDashboard(); break;
    case 1: drawBig("Voltage", voltage, "V", true); break;
    case 2: drawBig("Current", current, "A"); break;
    case 3: drawBig("Power", power, "W"); break;
    case 4: drawBig("Frequency", frequency, "Hz"); break;
    case 5: drawBig("Power Factor", pf, ""); break;
    case 6: drawBig("Energy", energyKWh, "kWh"); break;
    case 7: drawBig("RSSI", (float)WiFi.RSSI(), "dBm", true); break;
  }
}

// ================= SETUP =================
void setup() {
  setCpuFrequencyMhz(80); 
  Serial.begin(115200);
  pinMode(BTN_PIN, INPUT_PULLUP);
  
  Wire.begin(9, 11);
  Wire.setClock(100000);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) for(;;);

  showMessage("Booting...\nWiFi Manager Ready");
  setupWiFiManager();
  
  client.setServer(MQTT_SERVER, MQTT_PORT);
  float e = pzem.energy();
  energyStart = (isnan(e)) ? 0 : e;
  startTimeMillis = millis();
}

// ================= LOOP =================
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) connectMQTT();
    client.loop();
  }

  // --- LOGIKA TOMBOL ---
  if (digitalRead(BTN_PIN) == LOW) {
    if (!isPressing) {
      buttonPressedTime = millis();
      isPressing = true;
    }
    if (millis() - buttonPressedTime > LONG_PRESS_TIME) {
      showMessage("RESETTING WIFI...");
      WiFiManager wm;
      wm.resetSettings();
      delay(2000);
      ESP.restart();
    }
  } else {
    if (isPressing) {
      long duration = millis() - buttonPressedTime;
      if (duration > 50 && duration < 2000) {
        displayPage = (displayPage + 1) % totalPages;
        drawCurrentPage();
      }
      isPressing = false;
    }
  }

  // --- SENSOR & PUBLISH ---
  if (millis() - lastSensorRead > sensorInterval) {
    lastSensorRead = millis();
    voltage = pzem.voltage();
    current = pzem.current();
    power = pzem.power();
    frequency = pzem.frequency();
    pf = pzem.pf();
    float currentEnergy = pzem.energy();
    if (!isnan(currentEnergy)) {
      energyKWh = currentEnergy - energyStart;
      if (energyKWh < 0) energyKWh = 0;
    }

    if (client.connected()) {
      char payload[16];
      dtostrf(voltage, 4, 1, payload);    client.publish("caklutfi/power/pzem_main/voltage", payload, true);
      dtostrf(current, 5, 2, payload);    client.publish("caklutfi/power/pzem_main/current", payload, true);
      dtostrf(power, 5, 1, payload);      client.publish("caklutfi/power/pzem_main/power", payload, true);
      dtostrf(energyKWh, 6, 3, payload);  client.publish("caklutfi/power/pzem_main/energy", payload, true);
      // Tambahan PF dan Frequency
      dtostrf(pf, 4, 2, payload);         client.publish("caklutfi/power/pzem_main/pf", payload, true);
      dtostrf(frequency, 4, 1, payload);  client.publish("caklutfi/power/pzem_main/frequency", payload, true);
      unsigned long uptimeSec = millis() / 1000;
      sprintf(payload, "%lu", uptimeSec);
      client.publish("caklutfi/power/pzem_main/uptime", payload, true);
    }
    drawCurrentPage();
  }
  delay(10); 
}
