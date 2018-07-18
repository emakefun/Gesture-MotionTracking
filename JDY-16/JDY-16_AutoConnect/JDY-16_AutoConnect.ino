#include <SoftwareSerial.h>

#define BLE_JDY_RX 2
#define BLE_JDY_TX 3
#define AUTO_CONNECT_MAC "3CA5090A16B3"

String Receive = "";
String SendCmd = "";

SoftwareSerial BLE_JDY_16(BLE_JDY_TX, BLE_JDY_RX);
bool ConnectStatus = false;

void setup() {
    Serial.begin(9600);
    BLE_JDY_16.begin(9600);
    delay(2000);
    while (!ConnectStatus) {
        AutoConnect();
		    delay(1000);
    }
}
boolean AutoConnect() {
    Receive = "";
    BLE_JDY_16.print("AT+HOSTEN\r\n");
    delay(100);
	  while (BLE_JDY_16.available()) {
        Receive += (char)BLE_JDY_16.read();
        delay(2);
    }
    Serial.print(Receive);  // set host success
    if (Receive != "+HOSTEN=1\r\n") {
          Serial.print("ttt222");
          Receive = "";
          BLE_JDY_16.print("AT+HOSTEN1\r\n");
          while (BLE_JDY_16.available()) {
              Receive += (char)BLE_JDY_16.read();
              delay(2);
        }
        Serial.print(Receive);
    }
    delay(1000);
    Serial.print("test2:");
    BLE_JDY_16.print("AT+CONN");
	  BLE_JDY_16.print(AUTO_CONNECT_MAC);
	  BLE_JDY_16.print("\r\n");

    delay(1000);
    Receive = "";
    while (BLE_JDY_16.available()) {
        Receive += (char)BLE_JDY_16.read();
        delay(2);
    }
    Serial.print("test3:");
    Serial.print(Receive.substring(5,14));
    if (Receive.substring(5,14) != "CONNECTED") {
        Serial.println("connect fail");
       // return false;
    }
    Serial.print("connect success");
    ConnectStatus = true;
	  return true;
}

void loop() {
    char dat;
    while (Serial.available()) {
        dat = (char)Serial.read();
        BLE_JDY_16.print(dat);
        delay(2);
    }
    while (BLE_JDY_16.available()) {
        dat = (char)BLE_JDY_16.read();
        Serial.print(dat);
        delay(2);
    }
}
