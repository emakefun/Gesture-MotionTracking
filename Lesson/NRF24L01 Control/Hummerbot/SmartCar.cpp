#include "SmartCar.h"
#include "debug.h"
SmartCar::SmartCar(String name, byte type, byte addr, E_SMARTCAR_CONTROL_MODE control_mode = E_BLUETOOTH_CONTROL)
{
    SmartCarName = name;
    SmartCarType = type;
    Addr = addr;
    mControlMode = control_mode;
    mStatus = E_STOP;
    BatteryValue = 0;
    Temperature = 0;
    Speed = 0 ;
    Degree = 0;
}

SmartCar::~SmartCar()
{

}

void SmartCar::SetControlMode(E_SMARTCAR_CONTROL_MODE mode)
{
    if (mode < E_SMARTCAR_CONTROL_MAX && mode > 0)
    mControlMode = mode;
}

E_SMARTCAR_CONTROL_MODE SmartCar::GetControlMode(void)
{
    return mControlMode;
}

void SmartCar::SetSpeed(int8_t s)
{
    // DEBUG_LOG(DEBUG_LEVEL_INFO, "SetSpeed =%d \n", s);
    if (s > 100) {
        Speed = 100;
        return;
    } else if (s < 0) {
        Speed = 0;
        return;
    }
    Speed = s;
}

int SmartCar::GetSpeed(void)
{
    return Speed;
}
void SmartCar::SetSpeed_Left(int8_t s)
{
    // DEBUG_LOG(DEBUG_LEVEL_INFO, "SetSpeed =%d \n", s);
    if (s > 100) {
        Speed_left = 100;
        return;
    } else if (s < 0) {
        Speed_left = 0;
        return;
    }
    Speed_left = s;
}
void SmartCar::SetSpeed_Right(int8_t s)
{
    // DEBUG_LOG(DEBUG_LEVEL_INFO, "SetSpeed =%d \n", s);
    if (s > 100) {
        Speed_right = 100;
        return;
    } else if (s < 0) {
        Speed_right = 0;
        return;
    }
    Speed_right = s;
}

void SmartCar::SpeedUp(int8_t Duration = 5)
{
    SetSpeed(Speed + Duration);
    mStatus = E_SPEED_UP;
}

void SmartCar::SpeedDown(int8_t Duration = 5)
{
    SetSpeed(Speed - Duration);
    mStatus = E_SPEED_DOWN;
}

void SmartCar::SetStatus(E_SMARTCAR_STATUS status)
{
    mStatus = status;
}

E_SMARTCAR_STATUS SmartCar::GetStatus(void)
{
    return mStatus;
}

uint8_t SmartCar::GetBattery(void)
{
    return BatteryValue;


}
uint8_t SmartCar::GetTemperature(void)
{
    return Temperature;
}
