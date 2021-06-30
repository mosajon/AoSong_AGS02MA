/*!
 * @file readVoc.ino
 * @author Mosajon
 * @version  V1.0
 * @date  2021-01-31
 * edit like C:\Users\yourname\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\2.7.4\cores\esp8266\core_esp8266_si2c.cpp
 * unsigned int preferred_si2c_clock = 30000;

  *@file readVoc.ino
  *@brief Read the concentration of VOC and chip version. VOC unit: PPM (parts per million)
  * @n Experiment phenomenon: read the chip version before using, then read VOC concentration every 3 seconds and print it out
  * @n on seria port.
  * @n Resolution: 0.1ppm; Measuring range: 0~100PPM 
  *
  *@copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  *@licence     The MIT License (MIT)
  *@author [fengli](li.feng@dfrobot.com)
  *@version  V1.0
  *@date  2019-07-23
  *@get from https://www.dfrobot.com
  *@https://github.com/DFRobot/DFRobot_AGS01DB
*/
#include <AoSong_AGS02MA.h>
AoSong_AGS02MA AGS02MA(&Wire);

void setup() {

  Serial.begin(115200);
  //detect if the IIC communication works, return 0 if it is fine
  while (AGS02MA.begin() != 0) {
    Serial.println("failed to init chip, please check if the chip connection is fine ");
    delay(1000);
  }
  Serial.println("");
  Serial.print("the version of AGS02MA:");
  Serial.print("0x0");
  /*
    Function name: readSensorVersion()
    @brief Read chip version
  */
  Serial.println(/*version=*/AGS02MA.readSensorVersion(), HEX);
  AGS02MA.setMeasureMode(0);//0 - ppb;1 - ug/m3
}
void loop() {

  //Read VOC every 3 seconds, and the interval should be more than 2s since the sampling cycle of the chip >=2s. If it is set to less
  //than 2s, the reading will be one sampled at the last time. 
  //Please pre-heat the deivce for 120s if using it after a long time to get more accurate readings.
  float voc = AGS02MA.readVocPPB();//0 - ppb
  //float voc = AGS02MA.readVocUGM3();//1 - ug/m3
  if(voc >= 0){
    Serial.print("the concentration of Voc:");
    /*
      Function name: AGS02MA.readVocPPM()
      @brief Read the concentration of the harmful gas in air.
      @return Return the read VOC value, unit: ppm.
    */
    Serial.print(/*voc=*/voc);
    Serial.println(" PPB");//0 - ppb
    //Serial.println(" ug/m3");//1 - ug/m3
  }
  delay(3000);
  
}
