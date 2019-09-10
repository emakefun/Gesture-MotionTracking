/**************************************************************************************************
 *    _____    __      __        __           __        ______   _______   _        _    ___     _
 *  / _____)  /  \    /   \     /  \         / /       / _____) |   ____| | |      | |  |   \   | |
 * | |_____  | |\ \  / /| |    / /\ \       / /____   | |_____  |  |____  | |      | |  | |\ \  | |
 * |  _____| | | \ \/ / | |   / /  \ \     / / ___/   | |_____| |  |____| | |      | |  | | \ \ | |
 * | |_____  | |  \__/  | |  / /****\ \   / /\ \____  | |_____  |  |      | \______/ |  | |  \ \| |
 *  \______) |_|        |_| /_/      \_\ /_/  \_____\  \______) |__|       \ ______ /   |_|   \___|
 *
 *
 * Emakefun Tech firmware
 *
 * Copyright (C) 2015-2020
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, in version 3.
 * learn more you can see <http://www.gnu.org/licenses/>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.
 *
 *
 * [Title]    mpu6050 accel data read
 * [diagram]
 *           Arduino PIN VCC  ===================   VCC
 *           Arduino PIN GND  ===================   GND
 *           Arduino PIN A4   ===================   SDA
 *           Arduino PIN A5   ===================   SCL
 */

#include "Wire.h"

const int MPU_ADDR = 0x68;
const int nValCnt = 7;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  WriteMPUReg(0x6b, 0x01);  // set clock source
  WriteMPUReg(0x1b, 0x00);  // set MPU6050_GYRO_FS_250
  WriteMPUReg(0x1c, 0x00);  // set MPU6050_RA_ACCEL_CONFIG FS 2g
  WriteMPUReg(0x6B, 0x00);  // enable mpu6050
}

void loop()
{
    int readouts[nValCnt];
    ReadAccGyr(readouts);
    Serial.print(readouts[0]); Serial.print("\t");
    Serial.print(readouts[1]); Serial.print("\t");
    Serial.print(readouts[2]); Serial.print("\n");
    Serial.print(readouts[4]); Serial.print("\t");
    Serial.print(readouts[5]); Serial.print("\t");
    Serial.print(readouts[6]); Serial.print("\n");
    delay(1000);
}

void WriteMPUReg(int nReg, unsigned char nVal)
{
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(nReg);
    Wire.write(nVal);
    Wire.endTransmission(true);
}

unsigned char ReadMPUReg(int nReg)
{
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(nReg);
    Wire.requestFrom(MPU_ADDR, 1, true);
    Wire.endTransmission(true);
    return Wire.read();
}

void ReadAccGyr(int *pVals)
{
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B);
    Wire.requestFrom(MPU_ADDR, nValCnt * 2, true);
    Wire.endTransmission(true);
    for (long i = 0; i < nValCnt; ++i)
    {
        pVals[i] = Wire.read() << 8 | Wire.read();
    }
}