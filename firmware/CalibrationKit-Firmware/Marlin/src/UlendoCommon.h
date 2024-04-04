#pragma once

#include "MarlinCore.h"       // for configuration macros

#ifdef ULNDO_ACCELEROMETER_SUPPORT
  #include <SPI.h>            // for SPI objects
#endif


#ifdef ULNDO_ACCELEROMETER_SUPPORT
  #define ULNDO_ACC_SPI_SSEL PA4
  // #define ULNDO_USING_LIS3DH
  #define ULNDO_USING_ADXL345


  extern SPIClass SPI_EXP;
  extern SPISettings SPI_EXP_cfg;
#endif