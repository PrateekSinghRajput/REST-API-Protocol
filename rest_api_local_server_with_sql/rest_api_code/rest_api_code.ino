#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "config.h" // Include configuration

DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

void setup() {
  Serial.begin(115200);
  Serial.println("Started demo application");

  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.print("Connecting...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int lcdDotCount = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    lcd.setCursor(lcdDotCount, 1);
    lcd.print(".");
    lcdDotCount++;
    if (lcdDotCount > 15) {
      lcd.clear();
      lcd.print("Connecting...");
      lcdDotCount = 0;
    }
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.print("WiFi Connected!");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(2000);
  lcd.clear();
}

void send_http_request() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    float temperature = dht.readTemperature();
    float humidity    = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from DHT sensor!");
      lcd.clear();
      lcd.print("DHT Read Failed!");
      return;
    }

    String str_temperature = String(temperature, 2);
    String str_humidity    = String(humidity, 2);

    Serial.println("Temperature = " + str_temperature + " C");
    Serial.println("Humidity = " + str_humidity + " %");

    lcd.clear();
    lcd.print("Temp: " + str_temperature + "C");
    lcd.setCursor(0, 1);
    lcd.print("Hum: " + str_humidity + "%");

    String url = String("http://") + SERVER_IP + ":" + SERVER_PORT + "/add-data";
    Serial.println("URL: " + url);

    http.begin(url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String postData = "temperature=" + str_temperature + "&humidity=" + str_humidity;
    Serial.println("POST Data: " + postData);

    int httpCode = http.POST(postData);
    Serial.println("HTTP status code = " + String(httpCode));

    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();
        Serial.println("Server response: " + payload);
      }
    } else {
      Serial.printf("HTTP request failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  } else {
    Serial.println("WiFi not connected. Reconnecting...");
    lcd.clear();
    lcd.print("WiFi Disconnected!");
    lcd.setCursor(0, 1);
    lcd.print("Reconnecting...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  }
}

void loop() {
  send_http_request();
  delay(5000);
}
