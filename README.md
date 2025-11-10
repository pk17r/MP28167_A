
# MP28167_A

Arduino library for the MP28167_A buck boost dc dc converter.  
Library should work well for variants: MP28167_B and MP28167_N, though it hasn't been tested.  


## Description

Control IC using I2C.  
Vout can be set by setting Vref.  
Read datasheet for details.  

### Related

- https://www.monolithicpower.com/en/products/mp28167-a.html
- https://www.monolithicpower.com/en/evl28167-a-q-00a.html


## I2C

### Address

The sensor has a fixed I2C address: 0x60. 


## Interface

```cpp
#include "MP28167_A.h"
```


### Constructor

- **MP28167_A(TwoWire \*wire = Wire)** Constructor to set 
the address and optional Wire interface.
- **bool begin()** initializes the class.
returns true if the MP28167_A address is on the I2C bus.
Note: one needs to set **Wire.begin()** before calling **begin()**.
- **bool isConnected()** returns true if the MP28167_A address is on the I2C bus.


### Core Functions

- **void     setR1R2_kOhms(uint16_t r1, uint16_t r2)** set Feedback pin resistors R1, R2 in kOhms.
- **uint16_t getVout_mV()** get the set Vout Voltage in mV.
- **bool setVout_mV(uint16_t vout_mV)** set the desired Vout Voltage in mV.
- **uint16_t getVref_mV()** 
- **bool setVref_mV(uint16_t vref_mV)** 


## Support

If you appreciate my libraries, you can support the development and maintenance.
Improve the quality of the libraries by providing issues and Pull Requests, or
donate through PayPal or GitHub sponsors.

Thank you,

