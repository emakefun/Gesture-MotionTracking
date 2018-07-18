// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include "Wire.h"

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"
#include "ProtocolParser.h"
#include <SoftwareSerial.h>
#include "nRF24L01.h"

#define DEBUG_LEVEL DEBUG_LEVEL_ERR
#include "debug.h"

#define BLE_JDY_RX 2
#define BLE_JDY_TX 3

#define AUTO_CONNECT_MAC "3CA5090A1690"

MPU6050 accelgyro;

struct RAW_type{
    uint8_t x;
    uint8_t y;
    uint8_t z;
};

int16_t ax, ay, az;
int16_t gx, gy, gz;
struct RAW_type accel_self_enable ,accel_self_diable;
struct RAW_type accel_zero_offsent ,gyro_zero_offsent;

#define LED_PIN 13   //NANO LED PIN
bool blinkState = false;

const float pi = 3.1415926;
const float Rad = 57.3 ;  //180.0/pi;

float RwAcc[3];         // ACC raw data
float Gyro[3];          // 
float RwGyro[3];        // Gyro raw dat
float Awz[2];           // 
float RwEst[3];
int w = 0;
float tmpf = 0.0;
int currentTime, signRzGyro;
int lastTime = 0;
int interval = 0;
float wGyro = 10.0;

float AcceRatio = 16384.0;
float GyroRatio = 131.0*57.3;

float accx, accy, accz;
float gyrox, gyroy, gyroz;

float roll, pitch, yaw = 0;

byte readbyte(void);

Nrf24l mNrf24L01 = Nrf24l(10, 9);
bool ConnectStatus = false;
ST_PROTOCOL SendData;

float speed, degree;

byte readbyte(void)
{
     return;
}

int writedata(byte buf[], int len)
{
    mNrf24L01.setTADDR((uint8_t *)"hummer-bot");           //Set the receiver address
    mNrf24L01.payload = 12;
 /*   for(int i = 0; i < 12; i++) {
      Serial.print(buf[i], HEX);Serial.print(" ");
    }
    Serial.println("");*/
    mNrf24L01.send(buf);                //Send instructions, send random number value
    while (mNrf24L01.isSending()) delay(2);        //Until you send successfully, exit the loop
}

ProtocolParser Package(0xAA, 0x55, readbyte, writedata);

void normalize3DVec(float vector[3]) {
    float R;
    R = sqrt(vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2]);
    vector[0] /= R;
    vector[1] /= R;  
    vector[2] /= R;
}

float squared(float x) {
    return x*x;
} 

void readSensors(void)
{
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    RwAcc[0] = (ax-accel_zero_offsent.x);
    RwAcc[1] = (ay-accel_zero_offsent.y);
    RwAcc[2] = (az-accel_zero_offsent.z);
    Gyro[0] = (gx-gyro_zero_offsent.x);
    Gyro[1] = (gy-gyro_zero_offsent.y);
    Gyro[2] = (gz-gyro_zero_offsent.z);
}


void getInclination() {
  static boolean firstSample = true;
  readSensors();
  normalize3DVec(RwAcc);
 
  currentTime = millis();
  interval = currentTime - lastTime;
  lastTime = currentTime;
 
  if (firstSample /*|| Float.isNaN(RwEst[0])*/) {
    for (w=0;w<=2;w++) {
      RwEst[w] = RwAcc[w];
    }
  }
  else {
    if (abs(RwEst[2]) < 0.1) {
      for (w=0;w<=2;w++) {
        RwGyro[w] = RwEst[w];
      }
    }
    else {
      for (w=0;w<=1;w++) {
        tmpf = Gyro[w];
        tmpf *= interval / 1000.0f;
        Awz[w] = atan2(RwEst[w], RwEst[2]) * 180 / PI;
        Awz[w] += tmpf;
      }
      signRzGyro = ( cos(Awz[0] * PI / 180) >=0 ) ? 1 : -1;
 
      // http://starlino.com/imu_guide.html
      for (w=0;w<=1;w++) {
        RwGyro[0] = sin(Awz[0] * PI / 180);
        RwGyro[0] /= sqrt( 1 + squared(cos(Awz[0] * PI / 180)) * squared(tan(Awz[1] * PI / 180)) );
        RwGyro[1] = sin(Awz[1] * PI / 180);
        RwGyro[1] /= sqrt( 1 + squared(cos(Awz[1] * PI / 180)) * squared(tan(Awz[0] * PI / 180)) );
      }
      RwGyro[2] = signRzGyro * sqrt(1 - squared(RwGyro[0]) - squared(RwGyro[1]));
    }
    for (w=0;w<=2;w++) RwEst[w] = (RwAcc[w] + wGyro * RwGyro[w]) / (1 + wGyro);
 
    normalize3DVec(RwEst);
  }

  firstSample = false;
  roll = RwEst[1];
  pitch = RwEst[0];
}

void setup()
{
    int i ;
    int32_t ax_zero = 0,ay_zero = 0,az_zero = 0,gx_zero =0 ,gy_zero = 0,gz_zero = 0 ;
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();
    Serial.begin(9600);
    mNrf24L01.spi = &MirfHardwareSpi;
    mNrf24L01.init();
    // Set your own address (sender address) using 5 characters
    mNrf24L01.setRADDR((byte *)"ABCDE");
    mNrf24L01.channel = 90;              //Set the channel used
    mNrf24L01.config();
    
    // initialize device
    //DEBUG_LOG(DEBUG_LEVEL_INFO, "Initializing I2C devices...");
    accelgyro.initialize();
    delay(500) ;
    accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_8);
    // enable Accel Self Test
    accelgyro.setAccelXSelfTest(1); // enable accel self test
    accelgyro.setAccelYSelfTest(1);
    accelgyro.setAccelZSelfTest(1);
    delay(500);
    accel_self_enable.x = accelgyro.getAccelXSelfTestDate();
    accel_self_enable.y = accelgyro.getAccelYSelfTestDate();
    accel_self_enable.z = accelgyro.getAccelZSelfTestDate();

    accelgyro.setAccelXSelfTest(0); // disable accel self test
    accelgyro.setAccelYSelfTest(0);
    accelgyro.setAccelZSelfTest(0);
    accel_self_diable.x = accelgyro.getAccelXSelfTestDate();
    accel_self_diable.y = accelgyro.getAccelYSelfTestDate();
    accel_self_diable.z = accelgyro.getAccelZSelfTestDate();

    accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
    // verify connection
    // DEBUG_LOG(DEBUG_LEVEL_INFO, "Testing device connections...");
    // DEBUG_LOG(DEBUG_LEVEL_INFO, accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
    /* get zero accel/gyro value */
    for( i = 0 ; i < 200 ; i++){
        accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz); //get raw accel/gyro measurements 
        ax_zero += az ;
        ay_zero += ay ;
        az_zero += az ;
        gx_zero += gx ;
        gy_zero += gy ;
        gz_zero += gz ;
    }
    accel_zero_offsent.x = ax_zero/200;
    accel_zero_offsent.y = ay_zero/200;
    accel_zero_offsent.z = az_zero/200;
    gyro_zero_offsent.x = gx_zero/200;
    gyro_zero_offsent.y = gy_zero/200;
    gyro_zero_offsent.z = gz_zero/200;
    
    pinMode(LED_PIN, OUTPUT);
}

void Send_Direction(unsigned int degree)
{
    byte buffer[2];
    SendData.start_code = 0xAA;
    SendData.type = E_AURORA_RACING;
    SendData.addr = 0x01;
    SendData.function = E_ROBOT_CONTROL_DIRECTION;
    buffer[0] = (degree >> 8) & 0xFF;
    buffer[1] = degree & 0xFF;
    SendData.data = buffer;
    SendData.len = 8;
    SendData.end_code = 0x55;
    Package.SendPackage(&SendData, 2);
}

void Send_Speed(unsigned int Speed)
{
    SendData.start_code = 0xAA;
    SendData.type = E_AURORA_RACING;
    SendData.addr = 0x01;
    SendData.function = E_ROBOT_CONTROL_SPEED;
    SendData.data = (byte *)&Speed;
    SendData.len = 7;
    SendData.end_code = 0x55;
    Package.SendPackage(&SendData, 1);
}

float CalculateSpeed(float roll, float pitch)
{
    float inclination = asin(sqrt(roll*roll +pitch*pitch))*Rad;
    return inclination;
}

void HandInclination(void)
{
   static int count = 0;
   static int SendSpeed = 0, SendDegree = 90;
   count++;
   speed = CalculateSpeed(roll, pitch);
   if ((-0.2 <= pitch) && (pitch <= 0.2) && (-0.2 <= roll) && (roll <= 0.2)) {
        speed = 0;
        SendDegree += 90;
    } else if (pitch < 0 && roll < 0) {
         degree = atan(roll/pitch)*Rad;
         SendDegree += ((unsigned int)(degree/10))*10;
    } else  if (pitch > 0 &&  roll < 0) {
         degree = atan(-pitch/roll)*Rad + 90;
         SendDegree += ((unsigned int)(degree/10))*10;  
    } else  if (pitch > 0 && roll > 0) {
         degree = atan(roll/pitch)*Rad + 180;
         SendDegree += ((unsigned int)(degree/10))*10;
    } else  if (pitch < 0 &&  roll > 0) {
         degree = atan(-pitch/roll)*Rad + 270;
         SendDegree += ((unsigned int)(degree/10))*10;
    } else {
        SendDegree += 90;
    }
    SendSpeed = (unsigned int)(speed/20)*20;

    if (degree < 30 || degree > 330) {
        SendDegree = 0;
    }
    if ( count >= 3 ) {
        count = 0;
        //  Serial.print(SendDegree/3);Serial.print(",");
        //  Serial.print(SendSpeed);Serial.println();
        Send_Direction(SendDegree/3);
        delay(5);
        Send_Speed(SendSpeed);
        SendDegree = 0;
        SendSpeed = 0;
   }
}

void loop() {
    // get inclination measurements from device
    getInclination();
    HandInclination();
     delay(5);
}
