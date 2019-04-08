#include "Hummerbot.h"
#include "ProtocolParser.h"
#include "debug.h"

#define RIGHT_PWMB  10  // PWMB
#define RIGHT_DIRB  6   // DIRB  ---  right
#define LEFT_PWMA   9   // PWMA
#define LEFT_DIRA   5   // DIRA  ---  left

ProtocolParser *mProtocol = new ProtocolParser();
Hummerbot hbot(mProtocol, RIGHT_PWMB, RIGHT_DIRB, LEFT_PWMA, LEFT_DIRA);
decode_results results;

void setup()
{
    Serial.begin(9600);
	Serial.println("Get last update from https://github.com/keywish/keywish-hummer-bot");
    hbot.init();
    hbot.SetControlMode(E_NRF24L01_CONTROL);
    hbot.SetSpeed(0);
    hbot.KeepStop();
    hbot.SetNrf24L01Pin();
}

//========================= bluetooth
void HandleBluetoothRemote()
{
    if (mProtocol->ParserPackage())
    {
        switch(mProtocol->GetRobotControlFun())
        {   
            case E_INFO:
                break;
            case E_ROBOT_CONTROL_DIRECTION:
                hbot.Drive(mProtocol->GetRobotDegree());
                break;
            case E_ROBOT_CONTROL_SPEED:
                hbot.SetSpeed(mProtocol->GetRobotSpeed());
                break ;
            case E_VERSION:
                break;  
        }
    }
}
//========================= Nrf24L01
void HandleNrf24L01Remote()
{
    byte value[12] = {0};
    bool godata = false;
    // When the program is received, the received data is output from the serial port
    if ( hbot.mNrf24L01->dataReady()) { //When the program is received, the received data is output from the serial port
        hbot.mNrf24L01->getData(value);
        mProtocol->RecevData(value, 12);
    }
    if (mProtocol->ParserPackage())
    {
        switch(mProtocol->GetRobotControlFun()) {
            case E_ROBOT_CONTROL_DIRECTION:
                DEBUG_LOG(DEBUG_LEVEL_INFO, "E_ROBOT_CONTROL_DIRECTION \n");
                hbot.Drive(mProtocol->GetRobotDegree());
                break;
            case E_ROBOT_CONTROL_SPEED:
                DEBUG_LOG(DEBUG_LEVEL_INFO, "E_ROBOT_CONTROL_SPEED \n");
                hbot.SetSpeed(mProtocol->GetRobotSpeed());
                break ;
            default:
                break;  
        }
    }
}



void loop()
{         
   // mProtocol->RecevData();

    switch(hbot.GetControlMode())
    {
        case E_BLUETOOTH_CONTROL:
            HandleBluetoothRemote();
            DEBUG_LOG(DEBUG_LEVEL_INFO, "E_BLUETOOTH_CONTROL \n");
            break;
        case E_NRF24L01_CONTROL:
            HandleNrf24L01Remote();
           // DEBUG_LOG(DEBUG_LEVEL_INFO, "E_NRF24L01_CONTROL \n");
            break;
        default:
            break;
    }
}
