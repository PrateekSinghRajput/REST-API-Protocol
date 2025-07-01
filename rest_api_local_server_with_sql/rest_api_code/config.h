// config.h
#ifndef CONFIG_H
#define CONFIG_H

// Network credentials
const char* WIFI_SSID     = "Prateek";
const char* WIFI_PASSWORD = "justdoelectronics@#12345";

// Server details
const char* SERVER_IP   = "192.168.0.112";
const char* SERVER_PORT = "4000";

// DHT Sensor details
#define DHT_PIN  D1        // DHT data pin connected to GPIO 4
#define DHT_TYPE DHT22     // DHT 22 (AM2302)

// LCD details
const int LCD_COLS    = 16;
const int LCD_ROWS    = 2;
const int LCD_ADDRESS = 0x27; // Common I2C address for 16x2 LCD

#endif
