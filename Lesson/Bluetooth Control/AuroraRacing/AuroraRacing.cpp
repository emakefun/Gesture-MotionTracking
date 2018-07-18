#include "AuroraRacing.h"
#include "ProtocolParser.h"
#include "Sounds.h"
#include "debug.h"

// For single motor drive
// AuroraRacing::AuroraRacing(ProtocolParser *Package, uint8_t Servo_Pin = AR_SERVO_PIN, uint8_t bin1 = AR_BIN1_PIN, uint8_t bin2 = AR_BIN2_PIN, uint8_t pwmb = AR_PWMB_PIN, uint8_t standby = AR_STANBY_PIN):SmartCar("Aurora-Racing", E_AURORA_RACING, 0x01, E_BLUTOOTH_CONTROL)
AuroraRacing::AuroraRacing(ProtocolParser *Package, uint8_t Servo_Pin, uint8_t bin1, uint8_t bin2, uint8_t pwmb, uint8_t standby):SmartCar("Aurora-Racing", E_AURORA_RACING, 0x01, E_BLUTOOTH_CONTROL)
{
    this->Bin1Pin = bin1;
    this->Bin2Pin = bin2;
    this->PwmbPin = pwmb;
    this->StandbyPin = standby;
    MotorDriveMode = E_SINGLE_MODE;
    SetStatus(E_STOP);
    mProtocolPackage = Package;
    SetServoPin(Servo_Pin);
}
/*
// For dual motor drive
//AuroraRacing::AuroraRacing(ProtocolParser *Package, uint8_t ain1 = AR_AIN1_PIN, uint8_t ain2 = AR_AIN2_PIN, uint8_t pwma = AR_PWMA_PIN, uint8_t bin1 = AR_BIN1_PIN, uint8_t bin2 = AR_BIN2_PIN, uint8_t pwmb = AR_PWMB_PIN, uint8_t standby = AR_STANBY_PIN):SmartCar("Aurora-Racing", E_AURORA_RACING, 0x01, E_BLUTOOTH_CONTROL)
AuroraRacing::AuroraRacing(ProtocolParser *Package, uint8_t ain1, uint8_t ain2, uint8_t pwma, uint8_t bin1, uint8_t bin2, uint8_t pwmb, uint8_t standby):SmartCar("Aurora-Racing", E_AURORA_RACING, 0x01, E_BLUTOOTH_CONTROL)
{
    this->Ain1Pin = ain1;
    this->Ain2Pin = ain2;
    this->PwmaPin = pwma;
    this->Bin1Pin = bin1;
    this->Bin2Pin = bin2;
    this->PwmbPin = pwmb;
    this->StandbyPin = standby;
    SetStatus(E_STOP);
    MotorDriveMode = E_DUAL_MODE;
    mProtocolPackage = Package;
}*/

AuroraRacing::~AuroraRacing()
{
    delete mIrRecv;
    delete mBuzzer;
    delete mForwardServo;
    delete mPs2x;
    delete mRgb;
}

void AuroraRacing::init(void)
{
    //keep TB6612 AIN stop
    if (E_DUAL_MODE == MotorDriveMode) {
        pinMode(Ain1Pin, OUTPUT);
        digitalWrite(Ain1Pin, LOW);
        pinMode(Ain2Pin, OUTPUT);
        digitalWrite(Ain2Pin, LOW);
        pinMode(PwmaPin, OUTPUT);
        digitalWrite(PwmaPin, HIGH);
    } else {
        mForwardServo->attach(ServoPin, 10, 160);
        SetServoBaseDegree(90);
    }
    //keep TB6612 BIN stop
    pinMode(Bin1Pin, OUTPUT);
    digitalWrite(Bin1Pin, LOW);
    pinMode(Bin2Pin, OUTPUT);
    digitalWrite(Bin2Pin, LOW);
    pinMode(PwmbPin, OUTPUT);
    digitalWrite(PwmbPin, HIGH);

    //keep TB6612 Standby
    pinMode(StandbyPin, OUTPUT);
    digitalWrite(StandbyPin, LOW);
}

void AuroraRacing::GoForward(void)
{
    DEBUG_LOG(DEBUG_LEVEL_INFO, "GoForward\n");
    SetStatus(E_FORWARD);
    SetDirection(ServoBaseDegree);
    Drive(ServoBaseDegree);
}

void AuroraRacing::GoBack(void)
{
    DEBUG_LOG(DEBUG_LEVEL_INFO, "GoBack\n");
    SetStatus(E_BACK);
    // SetDirection(270);
    Drive(270);
}

void AuroraRacing::KeepStop(void)
{
    DEBUG_LOG(DEBUG_LEVEL_INFO, "KeepStop\n");
    SetDirection(ServoBaseDegree);
    digitalWrite(PwmbPin, 0);
    if (E_DUAL_MODE == MotorDriveMode) {
        digitalWrite(Ain1Pin, LOW);
        digitalWrite(Ain2Pin, LOW);
        digitalWrite(PwmaPin, HIGH);
    }
    digitalWrite(Bin1Pin, LOW);
    digitalWrite(Bin2Pin, HIGH);
    digitalWrite(StandbyPin, HIGH);
    SetStatus(E_STOP);
}

void AuroraRacing::TurnLeft(bool Runing)
{
    DEBUG_LOG(DEBUG_LEVEL_INFO, "TurnLeft = %d\n",Degree);
    if (Degree >= 90 && Degree <= 180) {
        Degree += 15;
    } else {
        Degree = 90;
    }
    if (Degree > 170) {
        Degree = 170;
    }
    SetDirection(Degree);
    SetStatus(E_LEFT);
    if (Runing)
    Drive(Degree);
}

void AuroraRacing::TurnRight(bool Runing)
{
    DEBUG_LOG(DEBUG_LEVEL_INFO, "TurnRight = %d\n",Degree);
    if (Degree >= 0 && Degree <= 90) {
        Degree -= 15;
    } else {
        Degree = 90;
    }
    if (Degree < 30) {
        Degree = 30;
    }
    SetDirection(Degree);
    SetStatus(E_RIGHT);
    if (Runing)
    Drive(Degree);
}

void AuroraRacing::Drive(void)
{
    Drive(Degree);
}

void AuroraRacing::Drive(int degree)
{
    DEBUG_LOG(DEBUG_LEVEL_INFO, "degree = %d speed = %d\n", degree, Speed);
    int value = (Speed/10)*25;	 //app contol hbot_speed is 0 ~ 100 ,pwm is 0~255
    if (degree >= 0 && degree <= 180) {
        if (E_DUAL_MODE == MotorDriveMode) {
            digitalWrite(Ain1Pin, HIGH);
            digitalWrite(Ain2Pin, LOW);
        }
        digitalWrite(Bin1Pin, HIGH);
        digitalWrite(Bin2Pin, LOW);
        digitalWrite(StandbyPin, HIGH);
    } else if (degree > 180 && degree <= 360) {
        if (E_DUAL_MODE == MotorDriveMode) {
            digitalWrite(Ain1Pin, LOW);
            digitalWrite(Ain2Pin, HIGH);
        }
        digitalWrite(Bin1Pin, LOW);
        digitalWrite(Bin2Pin, HIGH);
        digitalWrite(StandbyPin, HIGH);
    } else {
        KeepStop();
        return;
    }
    if (E_DUAL_MODE == MotorDriveMode) {
        analogWrite(PwmaPin, value);
    }
    analogWrite(PwmbPin, value);
}

void AuroraRacing::SetIrPin(uint8_t pin = AR_IR_PIN)
{
    IrPin = pin;
    mIrRecv = new IRremote (IrPin);
    mIrRecv->begin();  // Initialize the infrared receiver
}

void AuroraRacing::SetBuzzerPin(uint8_t pin = AR_BUZZER_PIN)
{
    BuzzerPin = pin;
    mBuzzer = new Buzzer();
    mBuzzer->setpin(BuzzerPin);
}

void AuroraRacing::SetServoPin(uint8_t Servo_Pin = AR_SERVO_PIN)
{
     mForwardServo = new Servo();
     ServoPin = Servo_Pin;
}

void AuroraRacing::SetServoBaseDegree(uint8_t base = 90)
{
    ServoBaseDegree = base;
}

void AuroraRacing::SetRgbPin(uint8_t pin = AR_RGB_PIN)
{
    RgbPin = pin;
    mRgb = new RGBLed(7, RgbPin);
    mRgb->setNumber(2);
   // mRgb->reset();
}

void AuroraRacing::LightOn(E_RGB_INDEX index = E_RGB_ALL, long Color = RGB_WHITE)
{
    // mRgb->reset();
    if (index == E_RGB_ALL) {
        mRgb->setColor(0, Color);
    } else {
        mRgb->setColor(index, Color);
    }
    mRgb->show();
}
void AuroraRacing::LightOff(E_RGB_INDEX index = E_RGB_ALL)
{
  //  mRgb->reset();
    if (index == E_RGB_ALL) {
        mRgb->setColor(0, 0, 0);
    } else {
        mRgb->setColor(index, 0);
    }
    mRgb->show();
}

void AuroraRacing::SetDirection(int degree)
{
    int servo_degree;
   // int diff = 90 - ServoBaseDegree;
    if (degree > 360 ||  degree < 0) {
        DEBUG_ERR("degree set error =%d \n", degree);
        return;
    }
    Degree = degree;
    if (degree == 90 || degree == 270) {
        servo_degree = ServoBaseDegree;
     } else if (degree >= 0 && degree <= 180) {
        servo_degree = ServoBaseDegree + 90 - degree;   // 180-degree-diff
    } else if (degree > 180 && degree <= 360) {
        servo_degree = degree - 270 + ServoBaseDegree;  // degree-180-diff
    }
    if (servo_degree > 140) {
        servo_degree = 140;
    } else if (servo_degree < 0) {
        servo_degree = 0;
    }
    DEBUG_LOG(DEBUG_LEVEL_INFO, "servo degree = %d\n", servo_degree);
    mForwardServo->write(servo_degree);
}

void AuroraRacing::SetInfraredTracingPin(uint8_t Pin1 = AR_INFRARED_TRACING_PIN1, uint8_t Pin2 = AR_INFRARED_TRACING_PIN2, uint8_t Pin3 = AR_INFRARED_TRACING_PIN3, uint8_t Pin4 = AR_INFRARED_TRACING_PIN4, uint8_t Pin5 = AR_INFRARED_TRACING_PIN5)
{
    static bool InfraredTracingInit = false;
    if (!InfraredTracingInit) {
        InfraredTracingPin1 = Pin1;
        InfraredTracingPin2 = Pin2;
        InfraredTracingPin3 = Pin3;
        InfraredTracingPin4 = Pin4;
        InfraredTracingPin5 = Pin5;
        mInfraredTracing = new InfraredTracing(InfraredTracingPin1, InfraredTracingPin2, InfraredTracingPin3, InfraredTracingPin4, InfraredTracingPin5);
        mInfraredTracing->begin();
        InfraredTracingInit = true;
    }
}

int AuroraRacing::SetPs2xPin(uint8_t clk = AR_PS2X_CLK, uint8_t cmd = AR_PS2X_CMD, uint8_t att = AR_PS2X_CS, uint8_t dat = AR_PS2X_DAT)
{
    static bool Ps2xInit = false;
    int error = 0 ;
    if (!Ps2xInit) {
        DEBUG_LOG(DEBUG_LEVEL_INFO, "SetPs2xPin\n");
        Ps2xClkPin = clk;
        Ps2xCmdPin = cmd;
        Ps2xAttPin = att;
        Ps2xDatPin = dat;
        mPs2x = new PS2X();
        //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
        //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
        error = mPs2x->config_gamepad(Ps2xClkPin, Ps2xCmdPin, Ps2xAttPin, Ps2xDatPin, false, false);

        if (error == 1) {
            DEBUG_LOG(DEBUG_LEVEL_ERR, "No controller found, check wiring\n");
        } else if(error == 2) {
            DEBUG_LOG(DEBUG_LEVEL_ERR, "Controller found but not accepting commands\n");
        } else if(error == 3) {
            DEBUG_LOG(DEBUG_LEVEL_ERR, "Controller refusing to enter Pressures mode, may not support it\n");
        } else if(error == 0) {
            DEBUG_LOG(DEBUG_LEVEL_INFO, "Found Controller, configured successful\n");
        }
        Ps2xInit = true;
    }
    return error;
}

int AuroraRacing::ResetPs2xPin(void)
{
    int error = mPs2x->config_gamepad(Ps2xClkPin, Ps2xCmdPin, Ps2xAttPin, Ps2xDatPin, false, false);

    if (error == 1) {
        DEBUG_LOG(DEBUG_LEVEL_ERR, "No controller found, check wiring\n");
    } else if(error == 2) {
        DEBUG_LOG(DEBUG_LEVEL_ERR, "Controller found but not accepting commands\n");
    } else if(error == 3) {
        DEBUG_LOG(DEBUG_LEVEL_ERR, "Controller refusing to enter Pressures mode, may not support it\n");
    } else if(error == 0) {
        DEBUG_LOG(DEBUG_LEVEL_INFO, "Found Controller, configured successful\n");
    }
    return error;
}

void AuroraRacing::Sing(byte songName)
{
    switch(songName) {
       case S_connection:
         mBuzzer->_tone(note_E5, 50, 30);
         mBuzzer->_tone(note_E6, 55, 25);
         mBuzzer->_tone(note_A6, 60, 10);
          //  _tone(9, 394);
       break;

       case S_disconnection:
         mBuzzer->_tone(note_E5,50,30);
         mBuzzer->_tone(note_A6,55,25);
         mBuzzer->_tone(note_E6,50,10);
       break;

       case S_buttonPushed:
         mBuzzer->bendTones (note_E6, note_G6, 1.03, 20, 2);
         delay(30);
         mBuzzer->bendTones (note_E6, note_D7, 1.04, 10, 2);
       break;

       case S_mode1:
          mBuzzer->bendTones (note_E6, note_A6, 1.02, 30, 10);  //1318.51 to 1760
       break;

       case S_mode2:
          mBuzzer->bendTones (note_G6, note_D7, 1.03, 30, 10);  //1567.98 to 2349.32
       break;

       case S_mode3:
         mBuzzer->_tone(note_E6,50,100); //D6
         mBuzzer->_tone(note_G6,50,80);  //E6
         mBuzzer->_tone(note_D7,300,0);  //G6
       break;

       case S_surprise:
         mBuzzer->bendTones(800, 2150, 1.02, 10, 1);
         mBuzzer->bendTones(2149, 800, 1.03, 7, 1);
       break;

       case S_OhOoh:
         mBuzzer->bendTones(880, 2000, 1.04, 8, 3); //A5 = 880
         delay(200);

         for (int i=880; i<2000; i=i*1.04) {
              mBuzzer->_tone(note_B5,5,10);
         }
       break;

       case S_OhOoh2:
          mBuzzer->bendTones(1880, 3000, 1.03, 8, 3);
         delay(200);

         for (int i=1880; i<3000; i=i*1.03) {
             mBuzzer->_tone(note_C6,10,10);
         }
       break;

       case S_cuddly:
          mBuzzer->bendTones(700, 900, 1.03, 16, 4);
          mBuzzer->bendTones(899, 650, 1.01, 18, 7);
       break;

       case S_sleeping:
          mBuzzer->bendTones(100, 500, 1.04, 10, 10);
          delay(500);
          mBuzzer->bendTones(400, 100, 1.04, 10, 1);
       break;

       case S_happy:
          mBuzzer->bendTones(1500, 2500, 1.05, 20, 8);
          mBuzzer->bendTones(2499, 1500, 1.05, 25, 8);
       break;

       case S_superHappy:
          mBuzzer->bendTones(2000, 6000, 1.05, 8, 3);
          delay(50);
          mBuzzer->bendTones(5999, 2000, 1.05, 13, 2);
       break;

       case S_happy_short:
          mBuzzer->bendTones(1500, 2000, 1.05, 15, 8);
         delay(100);
          mBuzzer->bendTones(1900, 2500, 1.05, 10, 8);
       break;

       case S_sad:
          mBuzzer->bendTones(880, 669, 1.02, 20, 200);
       break;

       case S_confused:
          mBuzzer->bendTones(1000, 1700, 1.03, 8, 2);
          mBuzzer->bendTones(1699, 500, 1.04, 8, 3);
          mBuzzer->bendTones(1000, 1700, 1.05, 9, 10);
       break;

       case S_fart1:
          mBuzzer->bendTones(1600, 3000, 1.02, 2, 15);
       break;

       case S_fart2:
          mBuzzer->bendTones(2000, 6000, 1.02, 2, 20);
       break;

       case S_fart3:
          mBuzzer->bendTones(1600, 4000, 1.02, 2, 20);
          mBuzzer->bendTones(4000, 3000, 1.02, 2, 20);
       break;
    }
}

void AuroraRacing::SendBatteryPackage(byte *battery_value)
{
    ST_PROTOCOL send_dat;
    send_dat.start_code = PROTOCOL_START_CODE;
    send_dat.type = 0x01;
    send_dat.addr = 0x01;
    send_dat.function = E_BATTERY;
    send_dat.data = battery_value;
    send_dat.end_code = PROTOCOL_END_CODE;
    mProtocolPackage->SendPackage(&send_dat,1);
}
