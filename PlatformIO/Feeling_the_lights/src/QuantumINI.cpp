#include "QuantumINI.h"

#include <Arduino.h>

#include <ArduinoOTA.h>
#include <WiFi.h>
#include <SD.h>
#include <ESPmDNS.h>
//#include <NetworkUdp.h>

#include "QuantumTFT.h"
#include "TFT_eff.h"
#include "QuantumOTH.h"

WiFiServer telnetServer(23);
WiFiClient telnetClient;

const char* ssid[]     = {"Hofi",      "FRITZ!Box 7583 TQ",    "BGE_DEV"}; // SSID de las redes
const char* password[] = {"22hofi22!", "26395030011048404128", "d3!!xA12v??"};

Adafruit_NeoPixel strip(N_LED, STRIP_PIN, NEO_GRB + NEO_KHZ800);

SPIClass spiSD(HSPI);

void wifi_init() {
  WiFi.mode(WIFI_STA);
  for (int i = 0; i < 3; i++) {
    Serial.print("Conectando a ");
    Serial.println(ssid[i]);
    WiFi.begin(ssid[i], password[i]);

    int timeout = 0;
    while (WiFi.status() != WL_CONNECTED && timeout < 10) {
      delay(1000);
      Serial.print(".");
      timeout++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("\r\nConectado a la red: ");
      Serial.println(ssid[i]);
      char versionText[80];
      snprintf(versionText, sizeof(versionText), "Conectado a %s", static_cast<const char*>(ssid[i]));
      print_tft(versionText, 10, 120, 's', 2000);
      break;
    }
    WiFi.disconnect();
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  char versionText[80];
  String ipString = WiFi.localIP().toString();
  snprintf(versionText, sizeof(versionText), "IP: %s", ipString.c_str());
  print_tft(versionText, 10, 120, 's', 2000);
}

void ota_init() {
  ArduinoOTA.setPort(3232);
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) type = "sketch";
      else                                    type = "filesystem";
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if      (error == OTA_AUTH_ERROR)    Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR)   Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR)     Serial.println("End Failed");
    });
  ArduinoOTA.begin();
}

#define SD_CS    36
#define SD_SCK   35
#define SD_MOSI  34  // AKA SPI TX
#define SD_MISO  33  // AKA SPI RX
void sd_init() {
  bool fw2update = false;
  spiSD.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, spiSD, 80 * 1000000))
    Serial.println("Card Mount Failed");
  else {
    Serial.println("Card Mount OK");
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
    fw2update = SD.exists("/firmware.bin");
    update_fw(fw2update);
    delay(200); 
  }
}

void tft_init(int blk_int) {
  tft.init();
  tft_set_brightnes(blk_int);
  tft.setRotation(1);
  tft.fillScreen(TFT_RED);
  delay(200);
  tft.fillScreen(TFT_GREEN);
  Serial.println("TFT ready");
}

void led_init() {
  pinMode(STRIP_PIN, OUTPUT);
  strip.begin();
  strip.setBrightness(25);
  Serial.println("LED Strip ready");
  delay(200);
}

void imu_init() {
  if (!imu.begin(Wire, QMI8658_L_SLAVE_ADDRESS, SENSOR_SDA, SENSOR_SCL))
    Serial.println("Failed to find QMI8658 - check your wiring!");
  imu.configAccelerometer(SensorQMI8658::ACC_RANGE_4G, SensorQMI8658::ACC_ODR_1000Hz, SensorQMI8658::LPF_MODE_0);
  imu.configGyroscope(SensorQMI8658::GYR_RANGE_64DPS, SensorQMI8658::GYR_ODR_896_8Hz, SensorQMI8658::LPF_MODE_3);
  imu.enableGyroscope();
  imu.enableAccelerometer();
  calIMU();
  Serial.println("Accel and Gyro ready");
  delay(200);
}

unsigned long t_last_int  = 0;
unsigned long t_0         = 0;
volatile bool btn_pressed = false;
int           eff_counter = 5;
const int     deb_delay   = 50;   // [ms]
const int     min_t       = 1000; // [ms]
bool          flag        = 0;

void IRAM_ATTR isr() {
  unsigned long t_1 = millis();

  if (t_1 - t_last_int > deb_delay) {
    t_last_int = t_1;

    if (!btn_pressed)
      t_0 = t_1;
    else {
      btn_pressed = false;
      unsigned long delta_t = t_1 - t_0;

      if (delta_t > min_t) flag = !flag;
      else                 eff_counter = (eff_counter % 6) + 1;
    }
    btn_pressed = !btn_pressed;
  }
}
/*void IRAM_ATTR isr() {
  unsigned long t_0 = millis();
  if (t_0 - t_last_int > deb_delay) {
    t_last_int = t_0;
    eff_counter = (eff_counter % 6) + 1;
  }
}*/

int mic_init(int micPin) {
  int sum = 0;
  for (int i = 0; i < 1000; i++)
    sum += analogRead(micPin);
  return ((int)sum/1000);
}