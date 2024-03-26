// การทดลองที่ 2

#include <TridentTD_LineNotify.h>
#include <DHT.h>
#define DHTPIN D2      //pin connect DHT
#define DHTTYPE DHT22  //DHT11, DHT22 type of Sensor
#define SSID "IoT-Gateway"
#define PASSWORD "0840445565"
#define LINE_TOKEN "9NUpDAAK2zfqbx0k3axS2X3RzuGjaSO3r1yhWvUqKRA"

DHT dht(DHTPIN, DHTTYPE);

bool then33 = false;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println(LINE.getVersion());
  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n", SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(400);
  }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());
  LINE.setToken(LINE_TOKEN);  // กำหนด Line Token
  dht.begin();
}
void loop() {
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int adc = analogRead(A0);
  if (isnan(t) || isnan(h)) {
    Serial.println("Error Reading DHT 22 !!!");
    delay(1000);
    return;
  } else {

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print("\t");
    Serial.print("Temp: ");
    Serial.println(t);
    Serial.println("...............................");
    if (t >= 33 && then33 == false) {
      then33 = true;
      LINE.notify("อุณหภูมิขณะนี้ " + String(t) + " องศา");
      LINE.notify("ความชื้นขณะนี้ " + String(h) + " %");
      Serial.print("LINE Send!");
    }else if(t<33){
      then33 = false;
    }
  }
  delay(5000);
}
