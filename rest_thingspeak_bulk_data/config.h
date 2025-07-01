#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// WiFi Credentials
const char* ssid = "Prateek";
const char* password = "justdoelectronics@#12345";

// Thingspeak Server URL for bulk update (replace channel ID as needed)
const char* serverName = "https://api.thingspeak.com/channels/3001098/bulk_update.json";

// Thingspeak Write API Key
const char* write_api_key = "KGB0LX5R30EJACXB";

// Timer delay for sending data (milliseconds)
const unsigned long timerDelay = 30000;

#endif
