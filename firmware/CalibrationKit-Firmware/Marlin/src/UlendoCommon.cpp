#include "UlendoCommon.h"

#ifdef ULNDO_ACCELEROMETER_SUPPORT

  #include <SPI.h>

                //(mosi, miso, sclk)
  SPIClass SPI_EXP(PA7,  PA6,  PA5); //ULNDOTODO: Rewrite to allow library control the chip select

  #ifdef ULNDO_USING_LIS3DH
    SPISettings SPI_EXP_cfg(8333333, MSBFIRST, SPI_MODE1);
  #elif defined(ULNDO_USING_ADXL345)
    SPISettings SPI_EXP_cfg(8333333, MSBFIRST, SPI_MODE3);
  #endif
#endif