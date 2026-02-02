/**************************************************************
   LOLIN S2 Power Monitor with OLED + MQTT + Button UI
**************************************************************/

#include <WiFi.h>
#include <PubSubClient.h>
#include <PZEM004Tv30.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ================= WIFI =================
#define WIFI_SSID     "ssid"
#define WIFI_PASSWORD "password"

// ================= MQTT =================
#define MQTT_SERVER "test.mosquitto.org"
#define MQTT_PORT   1883

WiFiClient espClient;
PubSubClient client(espClient);

// ================= PZEM =================
HardwareSerial pzemSerial(1);
#define PZEM_RX 7
#define PZEM_TX 5
PZEM004Tv30 pzem(pzemSerial, PZEM_RX, PZEM_TX);

// ================= OLED =================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ================= BUTTON =================
#define BTN_NEXT 18
unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 200;

// ================= SCREEN TIMEOUT =================
unsigned long lastInteraction = 0;
const unsigned long screenTimeout = 300000;
bool screenOn = true;

// ================= TIMERS =================
unsigned long lastSensorRead = 0;
const unsigned long sensorInterval = 5000;

// ================= DISPLAY =================
int displayPage = 0;
const int totalPages = 8;

// ================= ENERGY =================
float energyStart = 0;

// ================= STORED VALUES =================
float voltage, current, power, frequency, pf, energyKWh;

// =================================================
// WIFI + MQTT
void connectWiFi() { WiFi.begin(WIFI_SSID, WIFI_PASSWORD); while (WiFi.status() != WL_CONNECTED) delay(500); }
void connectMQTT() { while (!client.connected()) { client.connect("LOLIN_S2_PZEM_Node"); delay(500);} }

// =================================================
// DRAW FUNCTIONS
void drawDashboard() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  int boxW = 64;
  int boxH = 16;

  display.drawRect(0, 0, boxW, boxH, SSD1306_WHITE);
  display.drawRect(64, 0, boxW, boxH, SSD1306_WHITE);
  display.setCursor(4,4);   display.printf("V %.0fV", voltage);
  display.setCursor(68,4);  display.printf("I %.2fA", current);

  display.drawRect(0, 16, boxW, boxH, SSD1306_WHITE);
  display.drawRect(64, 16, boxW, boxH, SSD1306_WHITE);
  display.setCursor(4,20);  display.printf("P %.0fW", power);
  display.setCursor(68,20); display.printf("PF %.2f", pf);

  display.drawRect(0, 32, boxW, boxH, SSD1306_WHITE);
  display.drawRect(64, 32, boxW, boxH, SSD1306_WHITE);
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

  display.display();
}

void drawCurrentPage() {
  if (!screenOn) return;

  switch (displayPage) {
    case 0: drawDashboard(); break;
    case 1: drawBig("Voltage", voltage, "V", true); break;
    case 2: drawBig("Current", current, "A"); break;
    case 3: drawBig("Power", power, "W"); break;
    case 4: drawBig("Frequency", frequency, "Hz"); break;
    case 5: drawBig("Power Factor", pf, ""); break;
    case 6: drawBig("Energy", energyKWh, "kWh"); break;
    case 7: drawBig("RSSI", WiFi.RSSI(), "dBm"); break;
  }
}

void handleButton() {
  if (digitalRead(BTN_NEXT) == LOW && millis() - lastButtonPress > debounceDelay) {
    lastButtonPress = millis();
    lastInteraction = millis();
    screenOn = true;
    displayPage = (displayPage + 1) % totalPages;
    drawCurrentPage();
  }
}

void handleScreenTimeout() {
  if (millis() - lastInteraction > screenTimeout && screenOn) {
    display.clearDisplay();
    display.display();
    screenOn = false;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BTN_NEXT, INPUT_PULLUP);
  Wire.begin(9, 11);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  connectWiFi();
  client.setServer(MQTT_SERVER, MQTT_PORT);

  float e;
  do { e = pzem.energy(); delay(500); } while (isnan(e));
  energyStart = e;

  lastInteraction = millis();
}

void loop() {
  if (!client.connected()) connectMQTT();
  client.loop();

  handleButton();
  handleScreenTimeout();

  if (millis() - lastSensorRead > sensorInterval) {
    lastSensorRead = millis();

    // Membaca data dari sensor
    voltage = pzem.voltage();
    current = pzem.current();
    power = pzem.power();
    frequency = pzem.frequency();
    pf = pzem.pf();
    energyKWh = pzem.energy() - energyStart;
    if (energyKWh < 0) energyKWh = 0;

    // --- PROSES PUBLISH MQTT ---
    char payload[32];
    dtostrf(voltage, 6, 2, payload);    client.publish("caklutfi/power/pzem_main/voltage", payload, true);
    dtostrf(current, 6, 3, payload);    client.publish("caklutfi/power/pzem_main/current", payload, true);
    dtostrf(power, 6, 2, payload);      client.publish("caklutfi/power/pzem_main/power", payload, true);
    dtostrf(frequency, 6, 2, payload);  client.publish("caklutfi/power/pzem_main/frequency", payload, true);
    dtostrf(pf, 4, 2, payload);         client.publish("caklutfi/power/pzem_main/pf", payload, true);
    dtostrf(energyKWh, 8, 3, payload);  client.publish("caklutfi/power/pzem_main/energy", payload, true);

    sprintf(payload, "%lu", millis() / 1000);
    client.publish("caklutfi/power/pzem_main/uptime", payload, true);

    drawCurrentPage(); // Update layar setelah data dikirim
  }
}
