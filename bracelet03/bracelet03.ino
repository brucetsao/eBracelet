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



// uncomment "OUTPUT_READABLE_ACCELGYRO" if you want to see a tab-separated
// list of the accel X/Y/Z and then gyro X/Y/Z values in decimal. Easy to read,
// not so easy to parse, and slow(er) over UART.
#define OUTPUT_READABLE_ACCELGYRO

// uncomment "OUTPUT_BINARY_ACCELGYRO" to send all 6 axes of data as 16-bit
// binary, one right after the other. This is very fast (as fast as possible
// without compression or data loss), and easy to parse, but impossible to read
// for a human.
//#define OUTPUT_BINARY_ACCELGYRO



//We always have to include the library
//#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 10 is connected to LOAD 
 pin 11 is connected to the CLK
 We have only a single MAX72XX.
 */
 
  #define DinPin 8
 #define CsPin 9
 #define ClkPin 10
 #define chipno 5
 #define chips 5
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
int showwordno = 0 ;
void setup() {
  mariolen = sizeof(melody) / sizeof(int) ;
  mariopos = 0 ;
    pinMode(comtrolBtn,INPUT);
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  
       #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
accelgyro.initialize();
   Serial.begin(9600) ;
   Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

      lc.setStringFont5X7(0,0,showword[showwordno],6); //FontWidth=6

  for(int i=0;i<4;i++)
 { 
  lc.shutdown(i,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(i,8);
  /* and clear the display */
  lc.clearDisplay(i);
 }
  
  writeArduinoOnMatrix();
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
 
     lc.LeftRotate(map(px, -90, 90, 8, 1));
    MarioPlaying() ;
    delay(50);

 //delay(100);
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
/*
 This method will display the characters for the
 word "Arduino" one after the other on the matrix. 
 (you need at least 5x7 leds to see the whole chars)
 */
void writeArduinoOnMatrix() {
  /* here is the data for the characters */
  /*
  byte a[8]={B00000000,
             B00000000,
             B01000010,
             B01111110,
             B01000010,
             B00000000,
             B00000000, 
             B00000000};
  byte b[8]={B00000000,
             B00011100,
             B00100010,
             B01000100,
             B01000100,
             B00100010,
             B00011100,
             B00000000};
  byte c[8]={B00000000,
             B00000010,
             B00000010,
             B01111110,
             B00000010,
             B00000010,
             B00000000,
             B00000000};
  byte d[8]={B00000000,
             B00111100,
             B01000010,
             B01000010,
             B01000010,
             B00100100,
             B00000000,
             B00000000};
  byte e[8]={B00000000,
             B00000010,
             B00000010,
             B01111110,
             B00000010,
             B00000010,
             B00000000,
             B00000000};
  byte f[8]={B00000000,
             B01111110,
             B00000100,
             B00011000,
             B00000100,
             B01111110,
             B00000000,
             B00000000};
       */
        
  /* now display them one by one with a small delay */
//  lc.setCharFont5X7(0, 0, 'H');
//   lc.setCharFont5X7(1, 0, 'e');

  
      lc.setStringFont5X7(0,0,showword[showwordno],6); //FontWidth=6

#if 0  
  lc.setRow(0,0,a[0]);
  lc.setRow(0,1,a[1]);
  lc.setRow(0,2,a[2]);
  lc.setRow(0,3,a[3]);
  lc.setRow(0,4,a[4]);
  lc.setRow(0,5,a[5]);
  lc.setRow(0,6,a[6]);
  lc.setRow(0,7,a[7]);
   lc.setRow(1,0,b[0]);
  lc.setRow(1,1,b[1]);
  lc.setRow(1,2,b[2]);
  lc.setRow(1,3,b[3]);
  lc.setRow(1,4,b[4]);
  lc.setRow(1,5,b[5]);
  lc.setRow(1,6,b[6]);
  lc.setRow(1,7,b[7]);
#endif
  Serial.begin(9600);
  Serial.println(lc.status[0],BIN);
  Serial.println(lc.status[1],BIN);
  Serial.println(lc.status[2],BIN);
  Serial.println(lc.status[3],BIN);
  Serial.println(lc.status[4],BIN);
  Serial.println(lc.status[5],BIN);
  Serial.println(lc.status[6],BIN);
  Serial.println(lc.status[7],BIN);
  
//  char *a=new char(5);
}
/*
void LShift()
{
  byte firststatus=lc.status[0];
  lc.setRow(0,0,lc.status[1]);
  lc.setRow(0,1,lc.status[2]);
  lc.setRow(0,2,lc.status[3]);
  lc.setRow(0,3,lc.status[4]);
  lc.setRow(0,4,lc.status[5]);
  lc.setRow(0,5,lc.status[6]);
  lc.setRow(0,6,lc.status[7]);
   lc.setRow(0,7,lc.status[8]);
   lc.setRow(1,0,lc.status[9]);
  lc.setRow(1,1,lc.status[10]);
  lc.setRow(1,2,lc.status[11]);
  lc.setRow(1,3,lc.status[12]);
  lc.setRow(1,4,lc.status[13]);
  lc.setRow(1,5,lc.status[14]);
  lc.setRow(1,6,lc.status[15]);
  lc.setRow(1,7,firststatus);
  delay(100);
  
}

*/
void MPU6050_Get_Angle(float x,float y,float z,int *rx, int *ry, int *rz)
{
                        *rz=(int)(atan(sqrt((x*x+y*y))/z) * 180/3.14);
                        *rx=(int)(atan(x/sqrt((y*y+z*z))) * 180/3.14);
                        *ry=(int)(atan(y/sqrt((x*x+z*z)))  * 180/3.14);

    //    return res*1800/3.14;//把弧度转换成角度
}

void buzz(int targetPin, long frequency, long length) {
//  digitalWrite(13,HIGH);
    long delayValue = 1000000/frequency/2; // calculate the delay value between transitions
    //// 1 second's worth of microseconds, divided by the frequency, then split in half since
    //// there are two phases to each cycle
  long numCycles = frequency * length/ 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to 
  //// get the total number of cycles to produce
  for (long i=0; i < numCycles; i++){ // for the calculated length of time...
    digitalWrite(targetPin,HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin,LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
  digitalWrite(13,LOW);

}

