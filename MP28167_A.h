#pragma once
//    FILE: MP28167_A.h
//  AUTHOR: Prashant Kumar
// VERSION: 0.0.1
//    DATE: 2025-07-14
// PURPOSE: Arduino library for MP28167_A Buck-Boost Converter
//     URL: https://github.com/pk17r/MP28167_A
//
//  Read the datasheet for the details


#include "Arduino.h"
#include "Wire.h"


#define MP28167_A_LIB_VERSION              "0.0.1"

#define MP28167_A_I2C_ADDRESS         0x60

#define MP28167_A_VREF_L              0x00
#define MP28167_A_VREF_H              0x01
#define MP28167_A_VREF_GO             0x02
#define MP28167_A_IOUT_LIM            0x03
#define MP28167_A_CTL1                0x04
#define MP28167_A_CTL2                0x05
#define MP28167_A_STATUS              0x09
#define MP28167_A_INTERRUPT           0x0A
#define MP28167_A_MASK                0x0B
#define MP28167_A_ID1                 0x0C
#define MP28167_A_MFR_ID              0x27
#define MP28167_A_DEV_ID              0x28
#define MP28167_A_IC_REV              0x29

#define MP28167_A_CTL1_ENABLE                     0x80
#define MP28167_A_CTL1_DISABLE                    0x7F
#define MP28167_A_CTL1_FREQ_500kHz                0x00
#define MP28167_A_CTL1_FREQ_750kHz                0x04
#define MP28167_A_CTL1_HICCUP_MODE                0x40
#define MP28167_A_CTL1_LATCH_OFF_MODE             0x00

#define MP28167_A_STATUS_POWER_GOOD               0x80
#define MP28167_A_STATUS_CONSTANT_CURRENT         0x10

#define MP28167_A_INTERRUPT_OVER_CURRENT_ENTER    0x20

#define VOUT_MIN_mV         1000
#define VOUT_MAX_mV         20470
#define VREF_MIN_mV         80
#define VREF_MAX_mV         1637
#define VREF_REG_MIN        0
#define VREF_REG_MAX        2047

class MP28167_A
{
public:
  explicit MP28167_A(TwoWire *wire = &Wire);

  uint8_t     MP28167_A_begin();
  uint8_t     MP28167_A_isConnected();
  void     MP28167_A_setR1R2_kOhms(uint16_t r1, uint16_t r2);


  // Core functions
  void MP28167_A_enable();
  void MP28167_A_disable();
  uint8_t MP28167_A_CCMode();
  uint8_t MP28167_A_PG();
  uint8_t MP28167_A_OCP();

  uint16_t MP28167_A_getVout_mV();
  uint8_t MP28167_A_setVout_mV(uint16_t vout_mV);
  uint16_t MP28167_A_getVref_mV();
  uint8_t MP28167_A_setVref_mV(uint16_t vref_mV);


  uint8_t MP28167_A_setILim_mA(uint16_t IoutLim_mA);
  uint16_t MP28167_A_getILim_mA();


  //  DEBUG
  uint8_t MP28167_A_getRegister(uint8_t reg)  { return _readRegister(reg); };


private:

  uint8_t _readRegister(uint8_t reg);
  uint8_t _writeRegister(uint8_t reg, uint8_t value);

  uint16_t MP28167_A_VoutToVref_mV(uint16_t Vout_mV);
  uint16_t MP28167_A_VrefToVout_mV(uint16_t Vref_mV);

  uint8_t MP28167_A_getILimReg();

  TwoWire * _wire;
  uint16_t MP28167_A_R1 = 430;
  uint16_t MP28167_A_R2 = 107;
};


//  -- END OF FILE --

