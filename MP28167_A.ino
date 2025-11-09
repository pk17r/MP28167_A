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

const uint16_t delay_ms = 1000;

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
  delay(delay_ms);

  Serial.println();
  Serial.print("IOUT LIM(mA): ");
  Serial.println(mps28167a.getIoutLimit_mA());

  uint16_t IoutLim_mA_set = 250;
  Serial.print("Set IoutLim_mA: ");
  Serial.print(IoutLim_mA_set);
  Serial.print(" mA = ");
  Serial.println((mps28167a.setIoutLimit_mA(IoutLim_mA_set) ? "SUCCESS" : "FAILURE"));

  delay(delay_ms);
  Serial.println();
  Serial.print("IOUT LIM(mA): ");
  Serial.println(mps28167a.getIoutLimit_mA());

  delay(delay_ms);

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
  mps28167a.setVout_mV(20000);
  Serial.print("VOUT(mV): ");
  Serial.print(mps28167a.getVout_mV());
  Serial.print(", VREF(mV): ");
  Serial.print(mps28167a.getVref_mV());
  Serial.print("\t\tStatus Register: ");
  Serial.print(mps28167a.getRegister(MP28167_A_STATUS), BIN);
  Serial.print("\t\tInterrupt Register: ");
  Serial.println(mps28167a.getRegister(MP28167_A_INTERRUPT), BIN);

  delay(delay_ms);
}

void loop()
{
  mps28167a.disable();
  delay(delay_ms);

  mps28167a.enable();
  Serial.println();
  mps28167a.setVout_mV(3320);
  Serial.print("VOUT(mV): ");
  Serial.print(mps28167a.getVout_mV());
  Serial.print(", VREF(mV): ");
  Serial.print(mps28167a.getVref_mV());
  Serial.print("\t\tStatus Register: ");
  Serial.print(mps28167a.getRegister(MP28167_A_STATUS), BIN);
  Serial.print("\t\tInterrupt Register: ");
  Serial.println(mps28167a.getRegister(MP28167_A_INTERRUPT), BIN);

  Serial.print("VOUT(mV): ");
  Serial.print(mps28167a.getVout_mV());
  Serial.print(", powerGood: ");
  Serial.print(mps28167a.powerGood());
  Serial.print(", constantCurrentModeOn: ");
  Serial.print(mps28167a.constantCurrentModeOn());
  Serial.print(", overCurrentProtectionEvent: ");
  Serial.println(mps28167a.overCurrentProtectionEvent());

  

  delay(delay_ms);

  Serial.println();
  mps28167a.setVout_mV(5020);
  Serial.print("VOUT(mV): ");
  Serial.print(mps28167a.getVout_mV());
  Serial.print(", VREF(mV): ");
  Serial.print(mps28167a.getVref_mV());
  Serial.print("\t\tStatus Register: ");
  Serial.print(mps28167a.getRegister(MP28167_A_STATUS), BIN);
  Serial.print("\t\tInterrupt Register: ");
  Serial.println(mps28167a.getRegister(MP28167_A_INTERRUPT), BIN);

  Serial.print("VOUT(mV): ");
  Serial.print(mps28167a.getVout_mV());
  Serial.print(", powerGood: ");
  Serial.print(mps28167a.powerGood());
  Serial.print(", constantCurrentModeOn: ");
  Serial.print(mps28167a.constantCurrentModeOn());
  Serial.print(", overCurrentProtectionEvent: ");
  Serial.println(mps28167a.overCurrentProtectionEvent());


  delay(delay_ms);

  Serial.println();
  mps28167a.setVout_mV(15020);
  Serial.print("VOUT(mV): ");
  Serial.print(mps28167a.getVout_mV());
  Serial.print(", VREF(mV): ");
  Serial.print(mps28167a.getVref_mV());
  Serial.print("\t\tStatus Register: ");
  Serial.print(mps28167a.getRegister(MP28167_A_STATUS), BIN);
  Serial.print("\t\tInterrupt Register: ");
  Serial.println(mps28167a.getRegister(MP28167_A_INTERRUPT), BIN);

  Serial.print("VOUT(mV): ");
  Serial.print(mps28167a.getVout_mV());
  Serial.print(", powerGood: ");
  Serial.print(mps28167a.powerGood());
  Serial.print(", constantCurrentModeOn: ");
  Serial.print(mps28167a.constantCurrentModeOn());
  Serial.print(", overCurrentProtectionEvent: ");
  Serial.println(mps28167a.overCurrentProtectionEvent());


  delay(delay_ms);

  Serial.println();
  mps28167a.setVout_mV(800);
  Serial.print("VOUT(mV): ");
  Serial.print(mps28167a.getVout_mV());
  Serial.print(", VREF(mV): ");
  Serial.print(mps28167a.getVref_mV());
  Serial.print("\t\tStatus Register: ");
  Serial.print(mps28167a.getRegister(MP28167_A_STATUS), BIN);
  Serial.print("\t\tInterrupt Register: ");
  Serial.println(mps28167a.getRegister(MP28167_A_INTERRUPT), BIN);

  Serial.print("VOUT(mV): ");
  Serial.print(mps28167a.getVout_mV());
  Serial.print(", powerGood: ");
  Serial.print(mps28167a.powerGood());
  Serial.print(", constantCurrentModeOn: ");
  Serial.print(mps28167a.constantCurrentModeOn());
  Serial.print(", overCurrentProtectionEvent: ");
  Serial.println(mps28167a.overCurrentProtectionEvent());

  delay(delay_ms);
}


//  -- END OF FILE --
