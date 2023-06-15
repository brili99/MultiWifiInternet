#include <ESP8266WiFi.h>

#define HOST_API "homeiot.pcbjogja.com"

void connectToWiFi();
bool isReachable(const char *hostname);

const char *ssid[] = {"PCB Ex Route", "PCB Expres V3", "PCB Ex Route 2"}; // Array of WiFi SSIDs
const char *password[] = {"bismillah", "Bismillah123", "bismillah2"};     // Array of corresponding passwords
const int numNetworks = sizeof(ssid) / sizeof(ssid[0]);                   // Number of WiFi networks
int8 connectedNetwork = -1;

void setup()
{
  Serial.begin(9600);
  ESP.wdtEnable(5000);
  connectToWiFi(); // Connect to the available WiFi networks
}

void loop()
{
  // Check if WiFi is still connected
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi connection lost. Reconnecting...");
    connectToWiFi();
  }
  delay(1000);
  ESP.wdtFeed();
  // Your main code here...
}

void connectToWiFi()
{
  // Loop through each WiFi network
  for (int i = 0; i < numNetworks; i++)
  {
    ESP.wdtFeed();
    Serial.print("Connecting to ");
    Serial.println(ssid[i]);
    // Connect to the current WiFi network
    WiFi.begin(ssid[i], password[i]);
    delay(2000);

    for (uint8_t r = 0; r < 5; r++)
    {
      ESP.wdtFeed();
      // Check the connection status
      if (WiFi.status() == WL_CONNECTED)
      {
        ESP.wdtFeed();
        // Check if the WiFi network is reachable
        if (isReachable(HOST_API))
        {
          Serial.println("WiFi connected");
          Serial.print("IP address: ");
          Serial.println(WiFi.localIP());
          connectedNetwork = i;
          return;
        }
        else
        {
          Serial.println("WiFi network not reachable");
          WiFi.disconnect();
          break;
        }
      }
      else
      {
        Serial.println("WiFi connection failed");
      }
      delay(1000);
    }
  }
  Serial.println("Could not connect to any WiFi network");
}

bool isReachable(const char *hostname)
{
  WiFiClient client;
  if (!client.connect(hostname, 80))
  {
    Serial.print("Failed to connect to ");
    Serial.println(hostname);
    return false;
  }
  client.stop();
  return true;
}