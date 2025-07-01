#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <esp_wifi.h>
#include "config.h"  // Include the header file

// Variables for sensor simulation
unsigned long humidity_between_1_to_100;
unsigned long temperature_between_20_30;
unsigned long aqi_between_40_85;
unsigned long lastTime = 0;

// JSON Document buffer size
DynamicJsonDocument sensor_data(1024);

void setup() 
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(10000);
    Serial.println("Not Connected with Access Point ....");
  }

  Serial.println();
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Timer Delay : 30 Second for sending second unit");
}

void loop() 
{
  // Simulate sensor data
  humidity_between_1_to_100 = random(1, 101);
  temperature_between_20_30 = random(20, 31);
  aqi_between_40_85 = random(40, 86);

  Serial.print("Humidity: ");
  Serial.println(humidity_between_1_to_100);

  // Prepare JSON payload
  sensor_data.clear();
  sensor_data["write_api_key"] = write_api_key;

  sensor_data["updates"][0]["delta_t"] = 1;
  sensor_data["updates"][0]["field1"] = temperature_between_20_30;
  sensor_data["updates"][0]["field2"] = humidity_between_1_to_100;
  sensor_data["updates"][0]["field3"] = aqi_between_40_85;

  if ((millis() - lastTime) > timerDelay) 
  {
    if (WiFi.status() == WL_CONNECTED) 
    {
      HTTPClient http;
      http.begin(serverName);
      http.addHeader("Content-Type", "application/json");

      String json;
      serializeJson(sensor_data, json);

      int responsecode = http.POST(json);
      Serial.print("HTTP Response code: ");
      Serial.println(responsecode);

      http.end();
    } 
    else 
    {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
