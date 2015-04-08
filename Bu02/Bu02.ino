#include <LedControl.h>

#include "I2Cdev.h"
#include "MPU6050.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high
int16_t ax, ay, az;
int16_t gx, gy, gz;

#define OUTPUT_READABLE_ACCELGYRO

  #define DinPin 8
 #define CsPin 9
 #define ClkPin 10
 #define chipno 4
 #define chips 4

 
//LedControl lc=LedControl(12,10,11,9);
LedControl lc=LedControl(DinPin,ClkPin,CsPin,chipno ); 
/* we always wait a bit between updates of the display */
unsigned long delaytime=1000;

void clearchar() ;
void showchar(byte *chr,int chipnum) ;
void showalpha(char w,int chipnum) ;


void setup() {
   
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
    accelgyro.initialize();

   Serial.begin(9600) ;
   Serial.println("Mr. BU Start Here");
     Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

   for(int i=0;i<4;i++)
 { 
  lc.shutdown(i,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(i,8);
  /* and clear the display */
  lc.clearDisplay(i);
 }
  lc.setStringFont5X7(0,0,"Mr._BU_Start_Here",6);
   lc.LeftRotate(10);
}

void loop() { 
  
    int px,py,pz ;
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

 //LShift();//  writeArduinoOnMatrix();

     MPU6050_Get_Angle(ax,ay,az,&px,&py,&pz) ;
    Serial.print("****" ) ;
    Serial.print(px) ;
    Serial.print("\t") ;
    Serial.print(py) ;
    Serial.print("\t") ;
    Serial.print(pz) ;
    Serial.print("\t") ;
    Serial.print("\n") ;
     lc.LeftRotate(map(px, -90, 90, 1, 8));
   
    delay(50);
 
}

void MPU6050_Get_Angle(float x,float y,float z,int *rx, int *ry, int *rz)
{
                        *rz=(int)(atan(sqrt((x*x+y*y))/z) * 180/3.14);
                        *rx=(int)(atan(x/sqrt((y*y+z*z))) * 180/3.14);
                        *ry=(int)(atan(y/sqrt((x*x+z*z)))  * 180/3.14);

    //    return res*1800/3.14;//把弧度转换成角度
}

