//Transmitter program

#include <SPI.h>
#include "nRF24L01.h"
#include "MirfHardwareSpiDriver.h"
Nrf24l Mirf = Nrf24l(10, 9);
byte value[8] = {0xAA, 0x01, 0x01, 0x06, 0x23, 0x00, 0x2B, 0x55};
uint8_t  Addr = 0x01;

const uint64_t pipe = 0xE8E8F0F0E1LL;
void setup()
{
    Serial.begin(9600);
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();
    // Set your own address (sender address) using 5 characters
    Mirf.setRADDR((byte *)"ABCDE");
    Mirf.channel = 90;              //Set the channel used
    Mirf.config();
}

void loop()
{
	Mirf.setTADDR((uint8_t *)"hummer-bot");           //Set the receiver address
    Mirf.payload = sizeof(value);
	//value = random(255);                      //0-255 random number
	Mirf.send(value);                //Send instructions, send random number value
	Serial.println("Wait for sending.....");
	while (Mirf.isSending()) delay(2);        //Until you send successfully, exit the loop
	Serial.println("Send success:");
//	Serial.println(value);
	delay(1000);
}
