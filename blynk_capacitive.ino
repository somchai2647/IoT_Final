#define BLYNK_TEMPLATE_ID "TMPL6XN5WTu_5"
#define BLYNK_TEMPLATE_NAME "IoT Lab1"
#define BLYNK_AUTH_TOKEN "lIaU6XxuPZDulpLFTRJ3Pvs-JlkvK7yo"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

#define DHTPIN D4
#define DHTTYPE DHT22
#define LED_1 D1  //V4
#define LED_2 D2  //V3

char ssid[] = "Nice";
char pass[] = "Nice0800";
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

//-------------------------- LED ---------------------------------- //
BLYNK_WRITE(V3)
{
  int value = param.asInt();
  digitalWrite(LED_1, value);
 
  Blynk.virtualWrite(V5, value);
}

BLYNK_WRITE(V4)
{
  int value = param.asInt();
  digitalWrite(LED_2, value);
  Blynk.virtualWrite(V6, value);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  Blynk.syncAll();
}

// This function sends DHT22 sensor readings to Virtual Pins V0 and V1
void myTimerEvent()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int moistureValue = map(analogRead(A0), 0, 1023, 0, 100);

  if(isnan(moistureValue))
  {
    Serial.println("Error Reading Moisture !!!");
    delay(1000);
    return;
  }
  else if (isnan(t) || isnan(h))
  {
    Serial.println("Error Reading DHT 22 !!!");
    delay(1000);
    return;
  }
  else
  {
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print("\t");
    Serial.print("Temp: ");
    Serial.println(t);
    Serial.print("\t");
    Serial.print("Moisture: ");
    Serial.println(moistureValue);
    Serial.println("...............................");
    Blynk.virtualWrite(V0, t); //Temperature
    Blynk.virtualWrite(V1, h); //Humidity
    Blynk.virtualWrite(V2, moistureValue); //moistureValue

    if(moistureValue>65){
      digitalWrite(LED_1, 1);
      Blynk.virtualWrite(V3, 1);

    }else{
      digitalWrite(LED_1, 0);
      Blynk.virtualWrite(V3, 0);

    }

  }
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(3000, myTimerEvent);
  dht.begin();
}


void loop()
{
  Blynk.run();
  timer.run();
}
