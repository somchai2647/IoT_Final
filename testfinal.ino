// -------------------- Feb, 2024 ------------------
#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include "DHT.h"
#include <TridentTD_LineNotify.h>

#define DHTPIN 2
#define DHTTYPE DHT22  // DHT 22  (AM2302), AM2321

char ssid[] = "TOT2300_2.4";                        //ชื่อไวไฟ
char pass[] = "net2300LTE";                       //รหัสไวไฟ
unsigned long myChannelNumber = 2419668;         //เลข ID
const char* myWriteAPIKey = "32AA3JZZUXAME8LP";  //API KEY
#define LINE_TOKEN  "U3nVIvKpYHBZT0oygKu1s0fhxvwm6DgGxth7RwmUMme"

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect");
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      delay(5000);
    }
    Serial.println("\nConnected.");
  }
  ThingSpeak.begin(client);  // Initialize ThingSpeak

  Serial.println(F("DHTxx test!"));
  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(1000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);

  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if (x == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  // ตรวจสอบอุณหภูมิเกิน 33 องศา
    if (t > 33.0 && !notifiedHighTemp) {
      LINE.notify("อุณหภูมิสูงกว่า 33 องศา!!! อุณหภูมิปัจจุบัน: " + String(t) + " องศาเซลเซียส");
      notifiedHighTemp = true; // ตั้งค่าเป็น true เพื่อบอกว่าได้ทำการแจ้งเตือนไปแล้ว
    }
    // หากอุณหภูมิกลับไปต่ำกว่า 33 องศาและมีการแจ้งเตือนอุณหภูมิสูงมาก่อนหน้า
    else if (t <= 33.0 && notifiedHighTemp) {
      notifiedHighTemp = false; // ตั้งค่าเป็น false เพื่อเตรียมสำหรับการแจ้งเตือนอุณหภูมิสูงในครั้งถัดไป
  }
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.println(F("°F"));
}
