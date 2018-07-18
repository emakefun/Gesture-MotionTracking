#ifndef _AURORARACING_H_
#define _AURORARACING_H_

#define VERSION "AR-20180601"

#include <stdint.h>
#include "Arduino.h"
#include "SmartCar.h"
#include "IRremote.h"
#include "Servo.h"
#include "PS2X_lib.h"  //for v1.6
#include "Buzzer.h"
#include "RGBLed.h"
#include "InfraredTracing.h"
#include "ProtocolParser.h"

#define AR_IR_PIN 8
#define AR_AIN1_PIN 3
#define AR_AIN2_PIN 11
#define AR_PWMA_PIN 5
#define AR_BIN1_PIN 4
#define AR_BIN2_PIN 2
#define AR_PWMB_PIN 6
#define AR_STANBY_PIN 7

#define AR_SERVO_PIN 13

#define AR_RGB_PIN A3
#define AR_BUZZER_PIN 9

#define AR_INFRARED_TRACING_PIN1 A0
#define AR_INFRARED_TRACING_PIN2 A1
#define AR_INFRARED_TRACING_PIN3 A2
#define AR_INFRARED_TRACING_PIN4 A4
#define AR_INFRARED_TRACING_PIN5 A5

#define AR_PS2X_CLK A4
#define AR_PS2X_CMD A1
#define AR_PS2X_CS  A2
#define AR_PS2X_DAT A0

typedef enum
{
    E_SINGLE_MODE = 1,
    E_DUAL_MODE = 2,
} E_MOTOR_DRIVE_MODE;

typedef enum
{
    E_RGB_ALL = 0,
    E_RGB_RIGHT = 1,
    E_RGB_LEFT = 2
} E_RGB_INDEX;

class AuroraRacing : public SmartCar {

private :
    E_MOTOR_DRIVE_MODE MotorDriveMode;  // 1 :single motor drive mode 2:dual motor drive mode
    uint8_t Ain1Pin, Ain2Pin, PwmaPin, Bin1Pin, Bin2Pin, PwmbPin, StandbyPin;
    uint8_t ServoPin;   // For servo
    uint8_t IrPin;      // Infrared remoter pin
    uint8_t BuzzerPin;  // Buzzer pin
    uint8_t RgbPin;     // Rgb pin
    uint8_t InfraredTracingPin1, InfraredTracingPin2, InfraredTracingPin3, InfraredTracingPin4, InfraredTracingPin5;    // for Infrared tracing pin
    uint8_t Ps2xClkPin, Ps2xCmdPin, Ps2xAttPin, Ps2xDatPin;    // for Ps2 remoter
    uint8_t ServoBaseDegree; //correct degree for stop
    ProtocolParser *mProtocolPackage;

public :
  //  AuroraRacing(ProtocolParser *Package, uint8_t Servo_Pin = AR_SERVO_PIN, uint8_t bin1 = AR_BIN1_PIN, uint8_t bin2 = AR_BIN2_PIN, uint8_t pwmb = AR_PWMB_PIN, uint8_t standby = AR_STANBY_PIN);
  //  AuroraRacing(ProtocolParser *Package, uint8_t ain1 = AR_AIN1_PIN, uint8_t ain2 = AR_AIN2_PIN, uint8_t pwma = AR_PWMA_PIN, uint8_t bin1 = AR_BIN1_PIN, uint8_t bin2 = AR_BIN2_PIN, uint8_t pwmb = AR_PWMB_PIN, uint8_t standby = AR_STANBY_PIN);
    AuroraRacing(ProtocolParser *Package, uint8_t Servo_Pin, uint8_t bin1, uint8_t bin2, uint8_t pwmb, uint8_t standby);
  //  AuroraRacing(ProtocolParser *Package, uint8_t ain1, uint8_t ain2, uint8_t pwma, uint8_t bin1, uint8_t bin2, uint8_t pwmb, uint8_t standby);
    ~AuroraRacing();
    IRremote  *mIrRecv;
    Servo *mForwardServo;
    PS2X *mPs2x;
    Buzzer *mBuzzer;
    RGBLed *mRgb;
    InfraredTracing *mInfraredTracing;
    void GoForward(void);
    void GoBack(void);
    void TurnLeft(bool KeepStatic);
    void TurnRight(bool KeepStatic);
    void KeepStop(void);
    void Drive(void);
    void Drive(int degree);
    // void SetSpeed(uint8_t s);
    void SetDirection(int degree);
    void SetIrPin(uint8_t pin = AR_IR_PIN);
    void SetServoPin(uint8_t pin = AR_SERVO_PIN);
    void SetServoBaseDegree(uint8_t base = 90);
    void SetRgbPin(uint8_t pin = AR_RGB_PIN);
    void LightOn(E_RGB_INDEX index = E_RGB_ALL, long Color = RGB_WHITE);
    void LightOff(E_RGB_INDEX index = E_RGB_ALL);
    void SetBuzzerPin(uint8_t pin = AR_BUZZER_PIN);
    void Sing(byte songName);
    void SetInfraredTracingPin(uint8_t Pin1 = AR_INFRARED_TRACING_PIN1, uint8_t Pin2 = AR_INFRARED_TRACING_PIN2, uint8_t Pin3 = AR_INFRARED_TRACING_PIN3, uint8_t Pin4 = AR_INFRARED_TRACING_PIN4, uint8_t Pin5 = AR_INFRARED_TRACING_PIN5);
    int SetPs2xPin(uint8_t clk = AR_PS2X_CLK, uint8_t cmd = AR_PS2X_CMD, uint8_t att = AR_PS2X_CS, uint8_t dat = AR_PS2X_DAT);
    int ResetPs2xPin(void);
    void SendBatteryPackage(byte *battery_value);
    void init(void);
};

#endif  /* _AURORARACING_H_ */
