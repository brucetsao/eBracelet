#include <LedControl.h>
#include "pitches.h"

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
 #define melodyPin 7
 #define comtrolBtn 6
 
//LedControl lc=LedControl(12,10,11,9);
LedControl lc=LedControl(DinPin,ClkPin,CsPin,chipno ); 
/* we always wait a bit between updates of the display */
unsigned long delaytime=1000;

String showword[5] = {"Hackathon","BruceTsao","Samantha","Jack","Hardy"} ;


int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7, 
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0, 

  NOTE_C7, 0, 0, NOTE_G6, 
  0, 0, NOTE_E6, 0, 
  0, NOTE_A6, 0, NOTE_B6, 
  0, NOTE_AS6, NOTE_A6, 0, 

  NOTE_G6, NOTE_E7, NOTE_G7, 
  NOTE_A7, 0, NOTE_F7, NOTE_G7, 
  0, NOTE_E7, 0,NOTE_C7, 
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6, 
  0, 0, NOTE_E6, 0, 
  0, NOTE_A6, 0, NOTE_B6, 
  0, NOTE_AS6, NOTE_A6, 0, 

  NOTE_G6, NOTE_E7, NOTE_G7, 
  NOTE_A7, 0, NOTE_F7, NOTE_G7, 
  0, NOTE_E7, 0,NOTE_C7, 
  NOTE_D7, NOTE_B6, 0, 0
};

//Mario main them tempo
int tempo[] = {
  12, 12, 12, 12, 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12, 

  12, 12, 12, 12,
  12, 12, 12, 12, 
  12, 12, 12, 12, 
  12, 12, 12, 12, 

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};
// music for mario
int mariolen= 0 ;
int mariopos = 0 ;

void setup() {
    mariolen = sizeof(melody) / sizeof(int) ;
  mariopos = 0 ;
  int showwordno = 0 ;
  
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
 
 // lc.setStringFont5X7(0,0,"Mr._BU_Start_Here",6);
 lc.setStringFont5X7(0,0,showword[showwordno],6); //FontWidth=6
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
    
            if (digitalRead(comtrolBtn) == 0)
            {
                showwordno ++ ;
                  if (showwordno >=5)
                      {
                          showwordno= 0 ;
                      }
                  lc.setStringFont5X7(0,0,showword[showwordno],6); //FontWidth=6
            }
 
     lc.LeftRotate(map(px, -90, 90, 1, 8));
       MarioPlaying() ;
    delay(50);
 
}

void MPU6050_Get_Angle(float x,float y,float z,int *rx, int *ry, int *rz)
{
                        *rz=(int)(atan(sqrt((x*x+y*y))/z) * 180/3.14);
                        *rx=(int)(atan(x/sqrt((y*y+z*z))) * 180/3.14);
                        *ry=(int)(atan(y/sqrt((x*x+z*z)))  * 180/3.14);

    //    return res*1800/3.14;//把弧度转换成角度
}
void MarioPlaying()
{

int noteDuration = 1000/tempo[mariopos]; 
    tone(melodyPin, melody[mariopos],noteDuration);
 //buzz(melodyPin, melody[mariopos],noteDuration);
 if (mariopos >= mariolen)
   {
     mariopos = 0 ;
   }
   mariopos ++ ;
}
