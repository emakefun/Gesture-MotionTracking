#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
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
struct RAW_type accel_zero_offsent ,gyro_zero_offsent;

bool blinkState = false;
char str[512]; 

float GyroRatio = 131.0;
float gyrox,gyroy,gyroz;
        
void setup() {
    int i ;
    int32_t gx_zero =0 ,gy_zero = 0,gz_zero = 0 ;
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    Serial.begin(9600);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();
    delay(500) ;
    accelgyro.setFullScaleGyroRange(MPU6050_GYRO_FS_250);

    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
    for( i = 0 ; i < 200 ; i++)
    {
       accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
       gx_zero += gx;
       gy_zero += gy;
       gz_zero += gz;
    }
    gyro_zero_offsent.x = gx_zero/200 ;
    gyro_zero_offsent.y = gy_zero/200 ;
    gyro_zero_offsent.z = gz_zero/200;
	  sprintf(str,"%d,%d,%d\n", gyro_zero_offsent.x, gyro_zero_offsent.y, gyro_zero_offsent.z); 
    Serial.print(str); 
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    sprintf(str,"%d,%d,%d\n", gx-gyro_zero_offsent.x ,gy-gyro_zero_offsent.y, gz-gyro_zero_offsent.z);  
    Serial.print(str);
    gyrox = (float)(gx-gyro_zero_offsent.x)/GyroRatio;
    gyroy = (float)(gy-gyro_zero_offsent.y)/GyroRatio ;
    gyroz = (float)(gz-gyro_zero_offsent.z)/GyroRatio ;
    Serial.print(gyrox);Serial.print(" dec/s\t");
    Serial.print(gyroy);Serial.print(" dec/s\t");
    Serial.print(gyroz);Serial.print(" dec/s\n");

    delay(200);
    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
}
