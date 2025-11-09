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
  setR1R2(430, 107);
  if (! isConnected())
    return false;
  // set ALT pin Masks
  // _writeRegister(MP28167_A_MASK, 0x1F);
  // set 750kHz Frequency
  uint8_t ctrl1_register = _readRegister(MP28167_A_CTL1);
  ctrl1_register = (ctrl1_register | MP28167_A_CTL1_FREQ_750kHz);   // use 750kHz frequency
  ctrl1_register = (ctrl1_register & MP28167_A_CTL1_DISABLE);       // disable
  _writeRegister(MP28167_A_CTL1, ctrl1_register);
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
  // vref_min = VOUT_MIN_mV / Vref2VoutMultiplier / 0.8;
  // if(vref_min < VREF_MIN)
  //   vref_min = VREF_MIN;
  // vref_max = VOUT_MAX_mV / Vref2VoutMultiplier / 0.8;
  // if(vref_max > VREF_MAX)
  //   vref_max = VREF_MAX;
  // Serial.print("vref_min=");Serial.println(vref_min);
  // Serial.print("vref_max=");Serial.println(vref_max);
}


////////////////////////////////////////////////////////
//
//  CORE FUNCTIONS
//

void MP28167_A::enable() {
  // Serial.print("BEFORE interrupt_register=");Serial.println(_readRegister(MP28167_A_INTERRUPT), BIN);
  _writeRegister(MP28167_A_INTERRUPT, 0xFF);  // clear previous interrupts

  uint8_t ctrl1_register = _readRegister(MP28167_A_CTL1);
  // Serial.print("BEFORE ctrl1_register=");Serial.println(ctrl1_register, BIN);
  ctrl1_register = (ctrl1_register | MP28167_A_CTL1_ENABLE);
  _writeRegister(MP28167_A_CTL1, ctrl1_register);
}


void MP28167_A::disable() {
  uint8_t ctrl1_register = _readRegister(MP28167_A_CTL1);
  // Serial.print("BEFORE ctrl1_register=");Serial.println(ctrl1_register, BIN);
  ctrl1_register = (ctrl1_register & MP28167_A_CTL1_DISABLE);
  _writeRegister(MP28167_A_CTL1, ctrl1_register);
}


bool MP28167_A::constantCurrentModeOn() {
  uint8_t status_register = _readRegister(MP28167_A_STATUS);
  bool constant_current = ((status_register & MP28167_A_STATUS_CONSTANT_CURRENT) >> 4);
  return constant_current;
}


bool MP28167_A::powerGood() {
  uint8_t status_register = _readRegister(MP28167_A_STATUS);
  bool power_good = ((status_register & MP28167_A_STATUS_POWER_GOOD) >> 7);
  return power_good;
}


bool MP28167_A::overCurrentProtectionEvent() {
  uint8_t interrupt_register = _readRegister(MP28167_A_INTERRUPT);
  _writeRegister(MP28167_A_INTERRUPT, 0xFF);  // clear previous interrupts
  bool ocp = ((interrupt_register & MP28167_A_INTERRUPT_OVER_CURRENT_ENTER) >> 5);
  return ocp;
}


uint16_t MP28167_A::getVref_mV()
{
  uint8_t vref_l = _readRegister(MP28167_A_VREF_L);
  uint8_t vref_h = _readRegister(MP28167_A_VREF_H);
  uint16_t vref_register_val = ((vref_h << 3) | (vref_l & 0x07));
  uint16_t vref_mV = vref_register_val * 4 / 5;   // * 0.8
  return vref_mV;
}


bool MP28167_A::setVref_mV(uint16_t vref_mV)
{
  _writeRegister(MP28167_A_INTERRUPT, 0xFF);  // clear previous interrupts

  if(vref_mV < VREF_MIN_mV)
    vref_mV = VREF_MIN_mV;
  else if(vref_mV > VREF_MAX_mV)
    vref_mV = VREF_MAX_mV;

  uint16_t vref_register_val = vref_mV * 5 / 4;   // / 0.8

  if(vref_register_val < VREF_REG_MIN)
    vref_register_val = VREF_REG_MIN;
  else if(vref_register_val > VREF_REG_MAX)
    vref_register_val = VREF_REG_MAX;

  // if(vref_register_val > vref_max)
  //   vref_register_val = vref_max;
  // if(vref_register_val < vref_min)
  //   vref_register_val = vref_min;
  // Serial.print("vref_register_val=");Serial.println(vref_register_val);
  uint8_t vref_l = (vref_register_val & 0x0007);
  uint8_t vref_h = ((vref_register_val >> 3) & 0x00ff);
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


uint16_t MP28167_A::getVout_mV()
{
  uint16_t vref_mV = getVref_mV();
  return (uint16_t)(vref_mV * Vref2VoutMultiplier);
}


bool MP28167_A::setVout_mV(uint16_t vout_mV)
{
  float vref_mv = (float)vout_mV / Vref2VoutMultiplier;
  return setVref_mV(vref_mv);
}


bool MP28167_A::setIoutLimit_mA(uint16_t IoutLim_mA)
{
  uint8_t ilim_register_val = IoutLim_mA / (uint16_t)50;
  Serial.print("ilim_register_val=");Serial.println(ilim_register_val);
  ilim_register_val = (0x7F & ilim_register_val);
  Serial.print("ilim_register_val=");Serial.println(ilim_register_val);
  uint8_t result = _writeRegister(MP28167_A_IOUT_LIM, ilim_register_val);
  if(result == 0) return true;
  return false;
}


uint16_t MP28167_A::getIoutLimit_mA()
{
  uint8_t ilim_register_val = _readRegister(MP28167_A_IOUT_LIM);
  ilim_register_val = (0x7F & ilim_register_val);
  uint16_t ilim_mA = (uint16_t)ilim_register_val * 50;
  return ilim_mA;
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

