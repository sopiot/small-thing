#ifndef SMALL_THING_OTA_H_
#define SMALL_THING_OTA_H_

#include <SPI.h>
#include <WiFiNINA.h>
////////////////////
#include <ArduinoOTA.h>

#include "arduino_secrets.h"
#include "common.h"

void PrintWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void WiFi_Setup() {
  char ssid[] = SECRET_SSID;  // your network SSID (name)
  char pass[] = SECRET_PASS;  // your network password
  int status = WL_IDLE_STATUS;

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    return;
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP
    // network:
    status = WiFi.begin(ssid, pass);
  }

  // start the WiFi OTA library with internal (flash) based storage
  ArduinoOTA.begin(WiFi.localIP(), "Arduino", "password", InternalStorage);

  // you're connected now, so print out the status:
  PrintWifiStatus();
}

void WiFi_Setup(char* ssid, char* pass, char* device_name, char* upload_pass) {
  int status = WL_IDLE_STATUS;

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    return;
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP
    // network:
    status = WiFi.begin(ssid, pass);
  }

  // start the WiFi OTA library with internal (flash) based storage
  ArduinoOTA.begin(WiFi.localIP(), device_name, upload_pass, InternalStorage);

  // you're connected now, so print out the status:
  PrintWifiStatus();
}

static void SOPOTA() {
  ArduinoOTA.poll();
  SOPLOGLN(F("ArduinoOTA.poll() execute!"));
}

#endif