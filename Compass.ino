#include <Wire.h>
#include "LSM303D.h"
#include <stdint.h>
#include "Compensation.h"
#include "Calibration.h"

  String data;
  char menu;

  float f_offX; 
  float f_offY;
  float f_heading1;
  float f_heading2;
  
  V v_mag;
  V v_acc;

  LSM303D compass;
  Compensation compensation;
  Calibration calibration;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  while(!compass.enableDefault())
  {
    delay(500);  
  }
}

void loop() {

  if(Serial.available() == 0)
  {
    Serial.println("Wpisz w Monitor portu szeregowego: ");
    Serial.println("start kalibracja\t Aby rozpoczac proces kalibracji");
    Serial.println("start kompas\t     Aby rozpoczac proces kompasu");
    while(Serial.available() == 0)
    {
      delay(100);
    }
  }

  
  if(Serial.available() > 0)
    data = Serial.readStringUntil('\n');

  if (data == "start kalibracja")
    menu = '1';
  else if (data == "start kompas")
    menu = '2';
  else 
    menu = '3';
  
  switch(menu)
  {
    case '1':
      calibration.resetOffsets();
      while(Serial.available() == 0)
      {
        v_mag = compass.readRawMag(); //tu jest blad
 
        f_offX = calibration.findOffsetX(v_mag);
        f_offY = calibration.findOffsetY(v_mag);

        Serial.print((String)"X: " + v_mag.x + "\t" + "Y: " + v_mag.y + "\t");
        Serial.print((String)"Offset X: " + f_offX + "\t" + "Offset Y: " + f_offY + "\n");
        delay(100);
      }
      compass.setOffset(f_offX,f_offY);
    break;
    case'2':
      while(Serial.available() == 0)
      {
        v_mag = compass.readNormalizeMag();
        v_acc = compass.readNormalizeAcc();

        f_heading1 = compensation.noTiltCompensation(v_mag);
        f_heading2 = compensation.tiltCompensation (v_acc, v_mag);

        f_heading1 += compensation.declinationAngle(f_heading1, 4.0, 54.0, true );
        f_heading2 += compensation.declinationAngle(f_heading1, 4.0, 54.0, true );

        f_heading1 = compensation.fixAngle(f_heading1);
        f_heading2 = compensation.fixAngle(f_heading2);

        f_heading1 = compensation.convToDegrees(f_heading1);
        f_heading2 = compensation.convToDegrees(f_heading2);

        Serial.println((String)"bez kompensacji:\t" + f_heading1 + "\tz kompensacja:\t" + f_heading2);
        delay(100);
      }
    break;
    case '3':
    break;
    default:
    break;
  }
}
