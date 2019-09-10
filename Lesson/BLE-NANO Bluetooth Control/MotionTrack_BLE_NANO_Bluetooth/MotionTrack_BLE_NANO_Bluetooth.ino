#include "WirelessHandle.h"
#include "ProtocolParser.h"
#include "debug.h"

WirelessHandle mHandle(E_BLUTOOTH_MODE);
int count = 0;

byte readbyte(void) {
  if ((mHandle.GetControlMode() == E_BLUTOOTH_MODE) || (mHandle.GetControlMode() == E_WIFI_MODE)) {
    return Serial.read();
  } else {
    return;
  }
}

int writedata(byte buf[], int len)  
{
  if ((mHandle.GetControlMode() == E_BLUTOOTH_MODE) || (mHandle.GetControlMode() == E_WIFI_MODE)) {
    Serial.write(buf, len);
    Serial.flush();
    return len;
  } else {
    mHandle.mNrf24L01->setTADDR((uint8_t *)"WirelessHandle");           //Set the receiver address
    mHandle.mNrf24L01->payload = 12;
    mHandle.mNrf24L01->send(buf);
    while (mHandle.mNrf24L01->isSending())
      delay(2);
  }
}

ProtocolParser Package(0xAA, 0x55, readbyte, writedata);

int CalculateSpeed(float gx, float gy)
{
  return (sqrt(gx * gx + gy * gy) * 100);
}

void setup()
{
  Serial.begin(9600);
  mHandle.SetControlSelect(E_GRAVITY_CONTROL_MODE); //选择控制模式为MPU6050控制
  mHandle.SetControlMode(E_BLUTOOTH_MODE);          //E_BLUTOOTH_MODE or E_NRF24L01_MODE or E_WIFI_MODE
//  mHandle.InitNrf24l01(NRF24L01_CE, NRF24L01_CSN);  //设置NRF24LO1+与MCU的通讯引脚
  mHandle.InitMpu6050();
  mHandle.mProtocolPackage = &Package;
}

void loop() {
  int degree = mHandle.GetDegree();
  if (mHandle.GetControlSelect() == E_GRAVITY_CONTROL_MODE) 
  {
    mHandle.getInclination();
    if (count++ > 10) {
      count = 0;
      Package.SendSpeed(CalculateSpeed(mHandle.GetGravity(BT_Y), mHandle.GetGravity(BT_X)));
      Package.SendDirection(degree);
   //   Serial.print("degree======");
   //   Serial.println(degree);
    }
  } 
}
