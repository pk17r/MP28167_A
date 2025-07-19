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

#define MP28167_A_CTL1_FREQ_500kHz                0x00
#define MP28167_A_CTL1_FREQ_750kHz                0x04
#define MP28167_A_CTL1_HICCUP_MODE                0x40
#define MP28167_A_CTL1_LATCH_OFF_MODE             0x00

#define VOUT_MIN_mV         800
#define VOUT_MAX_mV         20400
#define VREF_MIN            1
#define VREF_MAX            2047

class MP28167_A
{
public:
  explicit MP28167_A(TwoWire *wire = &Wire);

  bool     begin();
  bool     isConnected();
  void     setR1R2(uint16_t r1, uint16_t r2);
  float    getVref2VoutMultiplier() { return Vref2VoutMultiplier; }


  //  Meta information
  uint8_t getManufacturerID();   //  should return 0x09 or 9 (decimal)
  uint8_t getDeviceID();            //  should return 0x58 or 88 (decimal)
  uint8_t getICRev();            //  should return 0x01 or 1 (decimal)


  uint16_t getVout();
  bool setVout(uint16_t vout_mV);
  float getVref();
  bool setVref(float vref_mV);

  bool setIoutLimit(float IoutLim);
  float getIoutLimit();


  //  DEBUG
  uint8_t getRegister(uint8_t reg)  { return _readRegister(reg); };


private:

  uint8_t _readRegister(uint8_t reg);
  uint8_t _writeRegister(uint8_t reg, uint8_t value);

  TwoWire * _wire;
  uint16_t R1 = 430;
  uint16_t R2 = 107;
  float Vref2VoutMultiplier = 5.0187;
  uint16_t vref_min = 199;
  uint16_t vref_max = 2047;
};


//  -- END OF FILE --

