//
//    FILE: MP28167_A.ino
//  AUTHOR: Prashant Kumar
// PURPOSE: demo control of Vout by changing Vref.
//     URL: https://github.com/pk17r/MP28167_A


#include "MP28167_A.h"


MP28167_A mps28167a;

const uint8_t kRegistersCount = 13;
const char kRegisterNames[kRegistersCount][20] =
{
  "   VREF_L: ",
  "   VREF_H: ",
  "  VREF_GO: ",
  " IOUT_LIM: ",
  "     CTL1: ",
  "     CTL2: ",
  "   STATUS: ",
  "INTERRUPT: ",
  "     MASK: ",
  "      ID1: ",
  "   MFR_ID: ",
  "   DEV_ID: ",
  "   IC_REV: "
};
const uint8_t kRegisterIds[kRegistersCount] =
{
  0,
  1,
  2,
  3,
  4,
  5,
  9,
  0x0A,
  0x0B,
  0x0C,
  0x27,
  0x28,
  0x29
};


const uint16_t delay_ms = 1000;

void setup()
{
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("MP28167_A_LIB_VERSION: ");
  Serial.println(MP28167_A_LIB_VERSION);

  Wire.begin();
  if (!mps28167a.MP28167_A_begin() )
  {
    Serial.println("Could not connect. Fix and Reboot");
    exit(1);
  }
  mps28167a.MP28167_A_setR1R2_kOhms(430, 107);    // in kOhm

  mps28167a.MP28167_A_setILim_mA(300 /*mA*/);
  Serial.print("IOUT LIM(mA): ");
  Serial.println(mps28167a.MP28167_A_getILim_mA());

  Serial.println("\n\tREGISTER\tVAL_HEX\tVAL_BIN");
  for (int r = 0; r < kRegistersCount; r++)
  {
    Serial.print("0x");
    Serial.print(kRegisterIds[r], HEX);
    Serial.print(" : ");
    Serial.print(kRegisterNames[r]);
    Serial.print('\t');
    uint8_t reg_val = mps28167a.MP28167_A_getRegister(kRegisterIds[r]);
    Serial.print(reg_val, HEX);
    Serial.print('\t');
    Serial.println(reg_val, BIN);
  }
  Serial.println();
}

void printsPerLine() 
{
  Serial.println();
  Serial.print("VOUT(mV): ");
  Serial.print(mps28167a.MP28167_A_getVout_mV());
  Serial.print(", VREF(mV): ");
  Serial.print(mps28167a.MP28167_A_getVref_mV());
  Serial.print("  Status:");
  Serial.print(mps28167a.MP28167_A_getRegister(MP28167_A_STATUS), BIN);
  Serial.print("  Interrupt:");
  Serial.print(mps28167a.MP28167_A_getRegister(MP28167_A_INTERRUPT), BIN);
  if(mps28167a.MP28167_A_PG())
    Serial.print("  PG");
  if(mps28167a.MP28167_A_CCMode())
    Serial.print("  CC");
  if(mps28167a.MP28167_A_OCP())
    Serial.print("  OCP");
}

void loop()
{
  mps28167a.MP28167_A_disable();
  printsPerLine();
  delay(delay_ms/2);
  printsPerLine();
  delay(delay_ms/2);

  mps28167a.MP28167_A_enable();
  mps28167a.MP28167_A_setVout_mV(1000);
  printsPerLine();
  delay(delay_ms/2);
  printsPerLine();
  delay(delay_ms/2);

  mps28167a.MP28167_A_setVout_mV(3320);
  printsPerLine();
  delay(delay_ms/2);
  printsPerLine();
  delay(delay_ms/2);

  mps28167a.MP28167_A_setVout_mV(5020);
  printsPerLine();
  delay(delay_ms/2);
  printsPerLine();
  delay(delay_ms/2);

  mps28167a.MP28167_A_setVout_mV(15020);
  printsPerLine();
  delay(delay_ms/2);
  printsPerLine();
  delay(delay_ms/2);
}


//  -- END OF FILE --
