const char ssid[] = "WiFi-SSID";
const char pass[] = "WiFi-Password";
char str_mac[18] = {0};

void wifi_setup()
{
  Serial.println("Initializing Wifi...");
  printMacAddress();

  //Serial.println("Scanning available networks...");
  //listNetworks();
    
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  Serial.println("WiFi Connected !");
  printWiFiStatusInfo();
}

void wifi_check()
{
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    ESP.restart();
  }
}

void listNetworks() {
  // scan for nearby networks:
  Serial.println("** Scan Networks **");
  byte numSsid = WiFi.scanNetworks();

  // print the list of networks seen:
  Serial.print("number of available networks:");
  Serial.println(numSsid);

  // print the network number and name for each network found:
  for (int thisNet = 0; thisNet<numSsid; thisNet++) {
    Serial.print(thisNet);
    Serial.print(") ");
    Serial.print(WiFi.SSID(thisNet));
    Serial.print("\tSignal: ");
    Serial.print(WiFi.RSSI(thisNet));
    Serial.print(" dBm");
    Serial.print("\tEncryption: ");
    Serial.println(WiFi.encryptionType(thisNet));
  }
}

void printWiFiStatusInfo()
{
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    
    byte* bssid;
    bssid = WiFi.BSSID();    
    Serial.print("BSSID: ");
    Serial.print(bssid[5],HEX);
    Serial.print(":");
    Serial.print(bssid[4],HEX);
    Serial.print(":");
    Serial.print(bssid[3],HEX);
    Serial.print(":");
    Serial.print(bssid[2],HEX);
    Serial.print(":");
    Serial.print(bssid[1],HEX);
    Serial.print(":");
    Serial.println(bssid[0],HEX);    
    Serial.print("RSSI: ");
    Serial.println(WiFi.RSSI());

    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("Mask: ");
    Serial.println(WiFi.subnetMask());
    Serial.print("Gateway: ");
    Serial.println(WiFi.gatewayIP());  
}

void printMacAddress() {
  // the MAC address of your Wifi shield
  byte mac[6];    

  // print your MAC address:
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  sprintf(str_mac,"%02X:%02X:%02X:%02X:%02X:%02X",mac[5],mac[4],mac[3],mac[2],mac[1],mac[0]);
  Serial.println(str_mac);
}

char* getStrMac()
{
  return str_mac;
}
