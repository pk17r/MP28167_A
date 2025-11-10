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


bool MP28167_A::MP28167_A_begin()
{
  if (! MP28167_A_isConnected())
    return false;
  // set ALT pin Masks
  // _writeRegister(MP28167_A_MASK, 0x1F);

  // set 750kHz Frequency and MP28167_A_disable converter
  // uint8_t ctrl1_register = _readRegister(MP28167_A_CTL1);
  // ctrl1_register = (ctrl1_register | MP28167_A_CTL1_FREQ_750kHz);   // use 750kHz frequency
  // ctrl1_register = (ctrl1_register & MP28167_A_CTL1_DISABLE);       // MP28167_A_disable
  _writeRegister(MP28167_A_CTL1, 0x74);   // 0x74 = b01110100 = CTL1 Register = EN / OCP-OVP-HICCUP_LATCH-OFF / DISCHG_EN / MODE_Forced-PWM_Auto-PFM-PWM / FREQ_00-500kHz_01-750kHz / 00_Reserved
  // set Vout to 1V after MP28167_A_disable - good practice
  MP28167_A_setVout_mV(1000);
  return true;
}


bool MP28167_A::MP28167_A_isConnected()
{
  _wire->beginTransmission(MP28167_A_I2C_ADDRESS);
  return ( _wire->endTransmission() == 0);
}


void MP28167_A::MP28167_A_setR1R2_kOhms(uint16_t r1, uint16_t r2)
{
  MP28167_A_R1 = r1;
  MP28167_A_R2 = r2;
}

uint16_t MP28167_A::MP28167_A_VoutToVref_mV(uint16_t Vout_mV)
{
  return (uint16_t)(((uint32_t)Vout_mV * (uint32_t)MP28167_A_R2) / (uint32_t)(MP28167_A_R1 + MP28167_A_R2));
}


uint16_t MP28167_A::MP28167_A_VrefToVout_mV(uint16_t Vref_mV)
{
  return (uint16_t)(((uint32_t)Vref_mV * (uint32_t)(MP28167_A_R1 + MP28167_A_R2)) / (uint32_t)MP28167_A_R2);
}


////////////////////////////////////////////////////////
//
//  CORE FUNCTIONS
//

void MP28167_A::MP28167_A_enable() {
  // Serial.print("BEFORE interrupt_register=");Serial.println(_readRegister(MP28167_A_INTERRUPT), BIN);
  _writeRegister(MP28167_A_INTERRUPT, 0xFF);  // clear previous interrupts

  uint8_t ctrl1_register = _readRegister(MP28167_A_CTL1);
  // Serial.print("BEFORE ctrl1_register=");Serial.println(ctrl1_register, BIN);
  ctrl1_register = (ctrl1_register | MP28167_A_CTL1_ENABLE);
  _writeRegister(MP28167_A_CTL1, ctrl1_register);
}


void MP28167_A::MP28167_A_disable() {
  uint8_t ctrl1_register = _readRegister(MP28167_A_CTL1);
  // Serial.print("BEFORE ctrl1_register=");Serial.println(ctrl1_register, BIN);
  ctrl1_register = (ctrl1_register & MP28167_A_CTL1_DISABLE);
  _writeRegister(MP28167_A_CTL1, ctrl1_register);
  // set Vout to 1V after MP28167_A_disable - good practice
  MP28167_A_setVout_mV(1000);
}


bool MP28167_A::MP28167_A_CCMode() {
  uint8_t status_register = _readRegister(MP28167_A_STATUS);
  bool constant_current = ((status_register & MP28167_A_STATUS_CONSTANT_CURRENT) >> 4);
  return constant_current;
}


bool MP28167_A::MP28167_A_PG() {
  uint8_t status_register = _readRegister(MP28167_A_STATUS);
  bool power_good = ((status_register & MP28167_A_STATUS_POWER_GOOD) >> 7);
  return power_good;
}


bool MP28167_A::MP28167_A_OCP() {
  uint8_t interrupt_register = _readRegister(MP28167_A_INTERRUPT);
  _writeRegister(MP28167_A_INTERRUPT, 0xFF);  // clear previous interrupts
  bool ocp = ((interrupt_register & MP28167_A_INTERRUPT_OVER_CURRENT_ENTER) >> 5);
  return ocp;
}


uint16_t MP28167_A::MP28167_A_getVref_mV()
{
  uint8_t vref_l = _readRegister(MP28167_A_VREF_L);
  uint8_t vref_h = _readRegister(MP28167_A_VREF_H);
  uint16_t vref_register_val = ((vref_h << 3) | (vref_l & 0x07));
  uint16_t vref_mV = vref_register_val * 4 / 5;   // * 0.8
  return vref_mV;
}


bool MP28167_A::MP28167_A_setVref_mV(uint16_t vref_mV)
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


uint16_t MP28167_A::MP28167_A_getVout_mV()
{
  return MP28167_A_VrefToVout_mV(MP28167_A_getVref_mV());
}


bool MP28167_A::MP28167_A_setVout_mV(uint16_t vout_mV)
{
  return MP28167_A_setVref_mV(MP28167_A_VoutToVref_mV(vout_mV));
}


bool MP28167_A::MP28167_A_setILim_mA(uint16_t IoutLim_mA)
{
  uint8_t ilim_register_val_desired = IoutLim_mA / (uint16_t)50;
  ilim_register_val_desired = (0x7F & ilim_register_val_desired);
  // Serial.print("ilim_register_val_desired=");Serial.println(ilim_register_val_desired);

  // read current IoutLimit
  uint8_t ilim_register_val_current = MP28167_A_getILimReg();

  // increase or decrease step by step
  while (ilim_register_val_current != ilim_register_val_desired)
  {
    uint8_t ilim_register_val_new = (ilim_register_val_current > ilim_register_val_desired ? (ilim_register_val_current - 1) : (ilim_register_val_current + 1));
    uint8_t result = _writeRegister(MP28167_A_IOUT_LIM, ilim_register_val_desired);
    // return if unsuccessful write
    if(result != 0)
      return false;
    // read current IoutLimit
    ilim_register_val_current = MP28167_A_getILimReg();
  }

  // Serial.print("ilim_register_val_current=");Serial.println(ilim_register_val_current);
  return true;
}


uint16_t MP28167_A::MP28167_A_getILim_mA()
{
  return (((uint16_t)MP28167_A_getILimReg()) * (uint16_t)50);
}


uint8_t MP28167_A::MP28167_A_getILimReg()
{
  return (0x7F & _readRegister(MP28167_A_IOUT_LIM));
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

