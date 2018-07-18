 /*
 * [Title]    mpu6050 accel data read
 * [diagram]
 *           Arduino PIN VCC  ===================   VCC
 *           Arduino PIN GND  ===================   GND
 *           Arduino PIN A4   ===================   SDA
 *           Arduino PIN A5   ===================   SCL
 */

#include "Wire.h"

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

// class default I2C address is 0x68
// AD0 low = 0x68 (default for InvenSense evaluation board) AD0 high = 0x69

#define LED_PIN 13

MPU6050 accelgyro;

struct RAW_type
{
    uint8_t x;
    uint8_t y;
    uint8_t z;
};

int16_t ax, ay, az;
int16_t gx, gy, gz;
struct RAW_type accel_self_enable ,accel_self_diable;
struct RAW_type accel_zero_offsent ,gyro_zero_offsent;
char str[512]; 

bool blinkState = false ;
float AcceRatio = 16384.0;
float accx,accy,accz;

void setup() {
    int i ;
    int32_t ax_zero = 0,ay_zero = 0,az_zero = 0 ;
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();
    Serial.begin(9600);
    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();
    delay(500) ;
    accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
    for( i = 0 ; i < 200 ; i++)
    {
       accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
       ax_zero += az ;
       ay_zero += ay ;
       az_zero += az-16384 ; //z accel static is 16384(1g)
    }
    accel_zero_offsent.x = ax_zero/200 ;
    accel_zero_offsent.y = ay_zero/200 ;
    accel_zero_offsent.z = az_zero/200 ;
    Serial.print(accel_zero_offsent.x); Serial.print("\t");
    Serial.print(accel_zero_offsent.y); Serial.print("\t");
    Serial.print(accel_zero_offsent.z); Serial.print("\n");
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    // read raw accel/gyro measurements from device
    delay(1000);
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    sprintf(str,"%d,%d,%d\n",ax-accel_zero_offsent.x, ay-accel_zero_offsent.y ,az-accel_zero_offsent.z);
    Serial.print(str);
    accx = (float)( ax-accel_zero_offsent.x )/AcceRatio;
    accy = (float)( ay-accel_zero_offsent.y )/AcceRatio ;
    accz = (float)( az-accel_zero_offsent.z )/AcceRatio ;
    Serial.print(accx);Serial.print("g\t");
    Serial.print(accy);Serial.print("g\t");
    Serial.print(accz);Serial.print("g\n");

    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
}
