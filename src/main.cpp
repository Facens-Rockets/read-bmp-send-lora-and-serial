#include <Arduino.h>
#include "heltec.h"

#define TINY_BME280_I2C
#include "TinyBME280.h"

#define LORA_BAND 868E6 

unsigned int counter = 0;
String rssi = "RSSI --";
String packSize = "--";
String packet ;

tiny::BME280 mySensor;

void setup() {
  Serial.begin(115200);
  Heltec.begin(true, true, true, true, LORA_BAND);

  mySensor.setI2CAddress(0x76);

  Heltec.display->init();
  Heltec.display->flipScreenVertically();   
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->clear();

  if (mySensor.begin() == false) {
    Serial.println("Error communicating with sensor, check wiring and I2C address");
    Heltec.display->drawString(0, 0, "Error communicating with \nsensor, \ncheck wiring and I2C address");
    Heltec.display->display();
    while(1){}
  }

  Serial.println("Heltec.LoRa Initial success!");
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->display();
  delay(3000);
}

void loop() {
  // send packet
  LoRa.beginPacket();
  
/*
 * LoRa.setTxPower(txPower,RFOUT_pin);
 * txPower -- 0 ~ 20
 * RFOUT_pin could be RF_PACONFIG_PASELECT_PABOOST or RF_PACONFIG_PASELECT_RFO
 *   - RF_PACONFIG_PASELECT_PABOOST -- LoRa single output via PABOOST, maximum output 20dBm
 *   - RF_PACONFIG_PASELECT_RFO     -- LoRa single output via RFO_HF / RFO_LF, maximum output 14dBm
*/
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  
  LoRa.print(mySensor.readFixedPressure() / 100.0);
  LoRa.endPacket();

  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  
  Heltec.display->drawString(0, 0, "Sending packet: ");
  Heltec.display->drawString(90, 0, (String)(mySensor.readFixedPressure() / 100.0));
  Heltec.display->display();

  Serial.println(mySensor.readFixedPressure() / 100.0);
}