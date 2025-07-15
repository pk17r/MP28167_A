//    FILE: MP28167_A.cpp
//  AUTHOR: Prashant Kumar
// VERSION: 0.0.1
//    DATE: 2025-07-14
// PURPOSE: Arduino library for MP28167_A Buck-Boost Converter
//     URL: https://github.com/pk17r/MP28167_A
//
//  Read the datasheet for the details


#include "MP28167_A.h"


////////////////////////////////////////////////////////
//
//  CONSTRUCTOR
//
MP28167_A::MP28167_A(TwoWire *wire)
{
  _wire        = wire;
}


bool MP28167_A::begin()
{
  if (! isConnected()) return false;
  return true;
}


bool MP28167_A::isConnected()
{
  _wire->beginTransmission(MP28167_A_I2C_ADDRESS);
  return ( _wire->endTransmission() == 0);
}


void MP28167_A::setR1R2(uint16_t r1, uint16_t r2)
{
  R1 = r1;
  R2 = r2;
  Vref2VoutMultiplier = (float)(R1 + R2) / R2;
}


////////////////////////////////////////////////////////
//
//  CORE FUNCTIONS
//
float MP28167_A::getVref()
{
  uint8_t vref_l = _readRegister(MP28167_A_VREF_L);
  uint8_t vref_h = _readRegister(MP28167_A_VREF_H);
  uint16_t vref = ((vref_h << 3) | (vref_l & 0x07));
  float vref_mV = vref * 0.8;
  return vref_mV;
}


bool MP28167_A::setVref(float vref_mV)
{
  uint16_t vref = vref_mV / 0.8;
  uint8_t vref_l = (vref & 0x0007);
  uint8_t vref_h = ((vref >> 3) & 0x00ff);
  uint8_t result1 = _writeRegister(MP28167_A_VREF_L, vref_l);
  uint8_t result2 = _writeRegister(MP28167_A_VREF_H, vref_h);
  if((result1 | result2) == 0)
  {
    // set GO bit
    uint8_t result = _writeRegister(MP28167_A_VREF_GO, 0x01);
    if(result == 0) return true;
    return false;
  }
  return false;
}


uint16_t MP28167_A::getVout()
{
  float vref_mV = getVref();
  return vref_mV * Vref2VoutMultiplier;
}


bool MP28167_A::setVout(uint16_t vout_mV)
{
  float vref_mv = (float)vout_mV / Vref2VoutMultiplier;
  return setVref(vref_mv);
}


////////////////////////////////////////////////////////
//
//  META INFORMATION
//
uint8_t MP28167_A::getManufacturerID()
{
  return _readRegister(MP28167_A_MFR_ID);
}


uint8_t MP28167_A::getDeviceID()
{
  return _readRegister(MP28167_A_DEV_ID);
}


uint8_t MP28167_A::getICRev()
{
  return _readRegister(MP28167_A_IC_REV);
}


////////////////////////////////////////////////////////
//
//  PRIVATE
//
uint8_t MP28167_A::_readRegister(uint8_t reg)
{
  _wire->beginTransmission(MP28167_A_I2C_ADDRESS);
  _wire->write(reg);
  _wire->endTransmission();

  _wire->requestFrom(MP28167_A_I2C_ADDRESS, (uint8_t)1);
  uint8_t value = _wire->read();
  return value;
}


uint8_t MP28167_A::_writeRegister(uint8_t reg, uint8_t value)
{
  _wire->beginTransmission(MP28167_A_I2C_ADDRESS);
  _wire->write(reg);
  _wire->write(value);
  return _wire->endTransmission();
}


//  -- END OF FILE --

