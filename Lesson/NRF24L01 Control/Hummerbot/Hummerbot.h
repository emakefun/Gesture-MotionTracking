#ifndef _HUMMERBOT_H_
#define _HUMMERBOT_H_
#include <stdint.h>
#include "Arduino.h"
#include "SmartCar.h"
#include "IRremote.h"
#include "IRremoteInt.h"
#include "PS2X_lib.h"
#include "ProtocolParser.h"
#include "Servo.h"
#include "InfraredTracing.h"
#include "nRF24L01.h"

#define HB_IR_PIN 12
#define HB_LEFT_POSITIVE_PIN 10
#define HB_LEFT_NEGATIVE_PIN 6
#define HB_RIGHT_POSITIVE_PIN 9
#define HB_RIGHT_NEGATIVE_PIN 5

#define HB_TRIGPIN 2
#define HB_ECHOPIN 3
#define HB_SERVOPIN 13

#define HB_INFRARED_AVOIDANCE_LEFT_PIN A3
#define HB_INFRARED_AVOIDANCE_RIGHT_PIN A4

#define HB_INFRARED_TRACING_PIN1 A0
#define HB_INFRARED_TRACING_PIN2 A1
#define HB_INFRARED_TRACING_PIN3 A2

#define HB_PS2X_CLK 11
#define HB_PS2X_CMD 7
#define HB_PS2X_ATT 8
#define HB_PS2X_DAT 4

#define HB_NRF24L01_CE 4
#define HB_NRF24L01_CSN 7
#define HB_NRF24L01_MOSI 11
#define HB_NRF24L01_MISO 12
#define HB_NRF24L01_SCK 13

class Hummerbot : public SmartCar {

private :
    uint8_t left_positive_pin, left_negative_pin, right_positive_pin, right_negative_pin;
    uint8_t TrigPin, EchoPin, SevoPin;   // For Ultrasonic Avoidance
    uint8_t IrPin;                       // Infrared remoter pin
    uint8_t InfraredAvoidanceLeftPin, InfraredAvoidanceRightPin;        // for Infrared Avoidance pin
    uint8_t InfraredTracingPin1, InfraredTracingPin2, InfraredTracingPin3;    // for Infrared tracing pin
    uint8_t Ps2xClkPin, Ps2xCmdPin, Ps2xAttPin, Ps2xDatPin;    // for Ps2 remoter
    uint8_t Nrf24L01CePin, Nrf24L01CsnPin;    // for Ps2 remoter
    ProtocolParser *mProtocolPackage;
    
public :
    Hummerbot(ProtocolParser *Package, uint8_t m1 = HB_LEFT_POSITIVE_PIN, uint8_t m2 = HB_LEFT_NEGATIVE_PIN, uint8_t m3 = HB_RIGHT_POSITIVE_PIN, uint8_t m4 = HB_RIGHT_NEGATIVE_PIN);
    ~Hummerbot();
    IRrecv *mIrRecv;
    PS2X *ps2x;
    InfraredTracing *mInfraredTracing;
    Nrf24l *mNrf24L01;
    float left_read;
    float right_read;
    int L_speed_con;
    int R_speed_con;
    void GoForward(void);
    void GoBack(void);
    void TurnLeft(void);
    void TurnRight(void);
    void TurnLeft_L_R(void);
    void TurnRight_L_R(void);
    void KeepStop(void);
    void Drive(int digree);
    void SetIrPin(uint8_t pin = HB_IR_PIN);
    void Ult_TurnRight(void);
    void Ult_TurnLeft(void);
    float Ultrasonic_Detection();
    void SetDirection(int degree);
    void SetUltrasonicPin(uint8_t Trig_Pin = HB_TRIGPIN, uint8_t Echo_Pin = HB_ECHOPIN, uint8_t Sevo_Pin = HB_SERVOPIN);
    void servo_angle(float angle);
    void SetInfraredPin(uint8_t Left_Pin = HB_INFRARED_AVOIDANCE_LEFT_PIN, uint8_t Right_Pin = HB_INFRARED_AVOIDANCE_RIGHT_PIN);
    void SetInfraredAvoidancePin(uint8_t Left_Pin = HB_INFRARED_AVOIDANCE_LEFT_PIN, uint8_t Right_Pin = HB_INFRARED_AVOIDANCE_RIGHT_PIN);
    void SetInfraredTracingPin(uint8_t Pin1 = HB_INFRARED_TRACING_PIN1, uint8_t Pin2 = HB_INFRARED_TRACING_PIN2, uint8_t Pin3 = HB_INFRARED_TRACING_PIN3);
    void SetPs2xPin(uint8_t clk = HB_PS2X_CLK, uint8_t cmd = HB_PS2X_CMD, uint8_t att = HB_PS2X_ATT, uint8_t dat = HB_PS2X_DAT);
    void SetNrf24L01Pin(uint8_t ce = HB_NRF24L01_CE, uint8_t csn = HB_NRF24L01_CSN);
    void SendBatteryPackage(byte *battery_value);
    void init(void);
};

#endif  /* _HUMMERBOT_H_ */
