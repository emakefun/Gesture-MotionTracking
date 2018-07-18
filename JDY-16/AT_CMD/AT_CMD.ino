#include <SoftwareSerial.h>

#define BLE_JDY_RX 2
#define BLE_JDY_TX 3

String Receive = "";
String SendCmd = "";

SoftwareSerial BLE_JDY_16(BLE_JDY_TX, BLE_JDY_RX); //RX=7,TX=8
void setup() {
    Serial.begin(9600);
    BLE_JDY_16.begin(9600);
}

void loop() {
    while (Serial.available()) {
        Receive = (char)Serial.read();
        BLE_JDY_16.print(Receive);
    }
    while (BLE_JDY_16.available()) {
        SendCmd = (char)BLE_JDY_16.read();
        Serial.print(SendCmd);
    }
}
