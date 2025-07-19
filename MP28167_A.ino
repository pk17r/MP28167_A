//
//    FILE: MP28167_A.ino
//  AUTHOR: Prashant Kumar
// PURPOSE: demo control of Vout by changing Vref.
//     URL: https://github.com/pk17r/MP28167_A


#include "MP28167_A.h"


MP28167_A mps28167a;

char names[6][20] =
{
  "   VREF_L: ",
  "   VREF_H: ",
  "  VREF_GO: ",
  " IOUT_LIM: ",
  "     CTL1: ",
  "     CTL2: "
};

const uint16_t R1 = 430;
const uint16_t R2 = 107;

void setup()
{
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("MP28167_A_LIB_VERSION: ");
  Serial.println(MP28167_A_LIB_VERSION);

  Wire.begin();
  if (!mps28167a.begin() )
  {
    Serial.println("Could not connect. Fix and Reboot");
  }

  Serial.print("\nSet R1: ");
  Serial.print(R1);
  Serial.print("kOhm, R2: ");
  Serial.print(R2);
  Serial.println("kOhm");
  mps28167a.setR1R2(R1, R2);

  Serial.print("\nVref2VoutMultiplier: ");
  Serial.println(mps28167a.getVref2VoutMultiplier());
  delay(1000);

  Serial.println();
  Serial.print("IOUT LIM(A): ");
  Serial.println(mps28167a.getIoutLimit(), 2);

  float ioutLim_set = 4.0;
  Serial.print("Set IoutLim: ");
  Serial.print(ioutLim_set, 2);
  Serial.print("A = ");
  Serial.println((mps28167a.setIoutLimit(ioutLim_set) ? "SUCCESS" : "FAILURE"));

  delay(1000);
  Serial.println();
  Serial.print("IOUT LIM(A): ");
  Serial.println(mps28167a.getIoutLimit(), 2);

  delay(3000);

  Serial.println("\n\tREGISTER\tVALUE_X\tVALUE_BIN");
  Serial.print('\t');
  Serial.print("   MFR_ID: ");
  Serial.print('\t');
  Serial.print(mps28167a.getManufacturerID(), HEX);
  Serial.print('\t');
  Serial.println(mps28167a.getManufacturerID(), BIN);
  Serial.print('\t');
  Serial.print("   DEV_ID: ");
  Serial.print('\t');
  Serial.print(mps28167a.getDeviceID(), HEX);
  Serial.print('\t');
  Serial.println(mps28167a.getDeviceID(), BIN);
  Serial.print('\t');
  Serial.print("   IC_REV: ");
  Serial.print('\t');
  Serial.print(mps28167a.getICRev(), HEX);
  Serial.print('\t');
  Serial.println(mps28167a.getICRev(), BIN);
  for (int r = 0; r < 6; r++)
  {
    Serial.print('\t');
    Serial.print(names[r]);
    Serial.print('\t');
    Serial.print(mps28167a.getRegister(r), HEX);
    Serial.print('\t');
    Serial.println(mps28167a.getRegister(r), BIN);
  }
  Serial.println();

  Serial.println();
  mps28167a.setVout(20000);
  Serial.print("VOUT(mV): ");
  Serial.print(mps28167a.getVout());
  Serial.print(", VREF(mV): ");
  Serial.print(mps28167a.getVref());
  Serial.print("\t\tStatus Register: ");
  Serial.print(mps28167a.getRegister(MP28167_A_STATUS), BIN);
  Serial.print("\t\tInterrupt Register: ");
  Serial.println(mps28167a.getRegister(MP28167_A_INTERRUPT), BIN);

  delay(3000);
}


void loop()
{
  Serial.println();
  mps28167a.setVout(3320);
  Serial.print("VOUT(mV): ");
  Serial.print(mps28167a.getVout());
  Serial.print(", VREF(mV): ");
  Serial.print(mps28167a.getVref());
  Serial.print("\t\tStatus Register: ");
  Serial.print(mps28167a.getRegister(MP28167_A_STATUS), BIN);
  Serial.print("\t\tInterrupt Register: ");
  Serial.println(mps28167a.getRegister(MP28167_A_INTERRUPT), BIN);

  delay(3000);

  Serial.println();
  mps28167a.setVout(5020);
  Serial.print("VOUT(mV): ");
  Serial.print(mps28167a.getVout());
  Serial.print(", VREF(mV): ");
  Serial.print(mps28167a.getVref());
  Serial.print("\t\tStatus Register: ");
  Serial.print(mps28167a.getRegister(MP28167_A_STATUS), BIN);
  Serial.print("\t\tInterrupt Register: ");
  Serial.println(mps28167a.getRegister(MP28167_A_INTERRUPT), BIN);

  delay(3000);
}


//  -- END OF FILE --
