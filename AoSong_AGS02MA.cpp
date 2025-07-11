/*!
 * @file AoSong_AGS02MA.cpp
 * @author Mosajon
 * @version  V1.0
 * @date  2021-01-31

 * @file DFRobot_AGS01DB.h
 * @brief Define the basic structure of class DFRobot_AGS01DB
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2019-07-13
 * @get from https://www.dfrobot.com
 * @https://github.com/DFRobot/DFRobot_AGS01DB
*/
#include"AoSong_AGS02MA.h"

AoSong_AGS02MA::AoSong_AGS02MA(TwoWire * pWire)
  : _pWire(pWire) {
}

int AoSong_AGS02MA::begin() {
  _pWire->begin();
  _pWire->setClock(30000);
  //check if the IIC communication works 
  if (_pWire->requestFrom(AGS02MA_IIC_ADDR, 1) != 1) {
    return -1;
  }
  return 0;
}

float AoSong_AGS02MA::readVocPPB() {
  uint8_t readCMD[1]={0};
  uint8_t data[5] = {0};
  int voc=-10.0;
  readCMD[0] = CMD_DATA_COLLECTION;
  int retries = 10;
  // when the returned data is wrong, request to get data again until the data is correct. 
  while(retries--) {
    writeCommand(readCMD, 1);
    readData(data, 5);
    //read data from the sensor, check if VOC data got from the check function is correct. 
    //if it is correct, retrun VOC concentration 
    if (checkCRC8(data, 4) == 1) {
      voc = data[1];
      voc <<= 8;
      voc |= data[2];
      voc <<= 8;
      voc |= data[3];
      break;
    } else {
      DBG("voc's Crc8 incorrect");
    }
  }
  return voc / 10.0;
}

float AoSong_AGS02MA::readVocUGM3() {
  uint8_t readCMD[1]={0};
  uint8_t data[5] = {0};
  int voc=-10.0;
  readCMD[0] = CMD_DATA_COLLECTION;
  int retries = 10;
  // when the returned data is wrong, request to get data again until the data is correct. 
  while(retries--) {
    writeCommand(readCMD, 1);
    readData(data, 5);
    //read data from the sensor, check if VOC data got from the check function is correct. 
    //if it is correct, retrun VOC concentration 
    if (checkCRC8(data, 4) == 1) {
      voc = data[1];
      voc <<= 8;
      voc |= data[2];
      voc <<= 8;
      voc |= data[3];
      break;
    } else {
      DBG("voc's Crc8 incorrect");
    }
  }
  return voc / 10.0;
}

int AoSong_AGS02MA::readSensorVersion() {
  uint8_t readCMD[1] = {0};
  uint8_t data[5] = {0};
  int version = 0;
  int retries = 10;
  readCMD[0] = CMD_GET_VERSION;
  while (retries--) {
    writeCommand(readCMD, 1);
    readData(data, 5);
    //read data from the sensor, check if the Version data got from the check function is correct.
    if (checkCRC8(data, 4) == 1) {
      version = data[3];
      return version;
    } else {
      DBG("version's Crc8 incorrect");
    }
  }
    return version;
}

bool AoSong_AGS02MA::checkCRC8(uint8_t *data, uint8_t Num) {
  uint8_t bit, byte, crc = 0xFF;
  // the data of the converted data byte should be identical with the data got from the check function.
  for (byte = 0; byte < Num; byte++)
  {
    crc ^= (data[byte]);
    for (bit = 8; bit > 0; --bit)
    {
      if (crc & 0x80) 
          crc = (crc << 1) ^ 0x31;
      else 
          crc = (crc << 1);
    }
  }
  //compare the caculated crc with the read crc to determine if the reading is correct
  if (crc == data[Num]) { 
    return true;
  } else {
    return false;
  }
}

void AoSong_AGS02MA::writeCommand(const void *pBuf, size_t size) {
  if (pBuf == NULL) {
    DBG("pBuf ERROR!! : null pointer");
  }

  uint8_t * _pBuf = (uint8_t *)pBuf;
  _pWire->beginTransmission(AGS02MA_IIC_ADDR);
  for (uint8_t i = 0; i < size; i++) {
    _pWire->write(_pBuf[i]);
   delay(3);
  }
  _pWire->endTransmission();
}

uint8_t AoSong_AGS02MA::readData(void *pBuf, size_t size) {
  if (pBuf == NULL) {
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
  //read the data returned by the chip
  _pWire->requestFrom(AGS02MA_IIC_ADDR, size);
  for (uint8_t i = 0 ; i < size; i++) {
    _pBuf[i] = _pWire->read();
  }
  _pWire->endTransmission();
  return 1;
}

void AoSong_AGS02MA::setMeasureMode(uint8_t Mode)
{
  if(Mode==0)//PPB
  {
    uint8_t dataMode[6] = {0x00,0x00,0xFF,0x00,0xFF,0x30};
    writeCommand(dataMode,6);
  }

  if(Mode==1)//ug/m3
  {
    uint8_t dataMode[6] = {0x00,0x02,0xFD,0x02,0xFD,0x00};    
    writeCommand(dataMode,6);
  }
  delay(2000);
}
