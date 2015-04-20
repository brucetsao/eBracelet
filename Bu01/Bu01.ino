#include <LedControl.h>
  #define DinPin 8
 #define CsPin 9
 #define ClkPin 10
 #define chipno 4
 #define chips 4

 
//LedControl lc=LedControl(12,10,11,9);
LedControl lc=LedControl(DinPin,ClkPin,CsPin,chipno ); 
/* we always wait a bit between updates of the display */
unsigned long delaytime=1000;


void setup() {

   Serial.begin(9600) ;
   Serial.println("Mr. BU Start Here");
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
  


lc.LeftRotate(2);
 
}

