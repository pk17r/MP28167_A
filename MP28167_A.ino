//
//    FILE: MP28167_A.ino
//  AUTHOR: Prashant Kumar
// PURPOSE: demo control of Vout by changing Vref.
//     URL: https://github.com/pk17r/MP28167_A


#include "MP28167_A.h"


MP28167_A mps28167a;

const uint8_t kRegistersCount = 9;
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
  "     MASK: "
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
  0x0B
};


const uint16_t R1 = 430;
const uint16_t R2 = 107;

const uint16_t delay_ms = 3000;

void setup()
{
  Wire.begin();
  bool setup_success = true;
  if (!mps28167a.begin() )
  {
    setup_success = false;
  }
  mps28167a.setVout_mV(1000);

  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("MP28167_A_LIB_VERSION: ");
  Serial.println(MP28167_A_LIB_VERSION);

  if(!setup_success) {
    Serial.println("Could not connect. Fix and Reboot");
    exit(1);
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

  uint16_t IoutLim_mA_set = 200;
  Serial.print("Set IoutLim_mA: ");
  Serial.print(IoutLim_mA_set);
  Serial.print(" mA = ");
  Serial.println((mps28167a.setIoutLimit_mA(IoutLim_mA_set) ? "SUCCESS" : "FAILURE"));

  delay(delay_ms);
  Serial.println();
  Serial.print("IOUT LIM(mA): ");
  Serial.println(mps28167a.getIoutLimit_mA());

  delay(delay_ms);

  Serial.println("\n\tREGISTER\tVALUE_HEX\tVALUE_BIN");
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
  for (int r = 0; r < kRegistersCount; r++)
  {
    Serial.print("0x");
    Serial.print(kRegisterIds[r], HEX);
    Serial.print(" : ");
    Serial.print(kRegisterNames[r]);
    Serial.print('\t');
    uint8_t reg_val = mps28167a.getRegister(kRegisterIds[r]);
    Serial.print(reg_val, HEX);
    Serial.print('\t');
    Serial.println(reg_val, BIN);
  }
  Serial.println();

  Serial.println();
  mps28167a.setVout_mV(6000);
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
  mps28167a.setVout_mV(1000);
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
}


//  -- END OF FILE --
