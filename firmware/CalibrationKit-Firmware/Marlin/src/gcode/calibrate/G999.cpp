#include "../../UlendoCommon.h"
#include "../../MarlinCore.h"
#include "../../module/stepper.h"
#include "../../HAL/shared/Delay.h" // for delay in chip select timing

#include "../gcode.h"

#if ENABLED(ULNDO_ACCELEROMETER_SUPPORT)

static void ulndoRegWrite(uint8_t addr, uint8_t b){
    extDigitalWrite(ULNDO_ACC_SPI_SSEL, LOW);
    DELAY_NS(5);
    
    SPI_EXP.transfer(addr);
    SPI_EXP.transfer(b);
    
    DELAY_NS(5);
    extDigitalWrite(ULNDO_ACC_SPI_SSEL, HIGH);
    DELAY_NS(300); // 2x ADXL minimum time
  }
  
/* 
 * Called by G-code G999 to toggle whether to send the accelerometer data
*/ 

void GcodeSuite::G999() {
    
    if (parser.string_arg){    
           
        int8_t collection_mode = atoi(parser.string_arg);              

        if (collection_mode == 30 ){
            // Initialize SPI and configure the accelerometer
            _SET_MODE(ULNDO_ACC_SPI_SSEL, OUTPUT);
            extDigitalWrite(ULNDO_ACC_SPI_SSEL, HIGH);
            SPI_EXP.beginTransaction(SPI_EXP_cfg);
            DELAY_US(1000); // Precautionary
            #ifdef ULNDO_USING_ADXL345
                ulndoRegWrite(0x2C, 0x0E);
                ulndoRegWrite(0x31, 0x01);
                ulndoRegWrite(0x2D, 0x08);
            #elif defined(ULNDO_USING_LIS3DH)
                // ULNDO_TODO
            #endif
            // SERIAL_ECHO(30);
        }
        else if (collection_mode == 40 ){
            // Ebable sine wave generation
            stepper.collect_stepperandaccel_data = true;  
            // SERIAL_ECHO(40);            
        }
        else if (collection_mode == 50 ){
            // Just record data
            stepper.transmit_accelerometer_data = true;
        }
        else if (collection_mode == 60 ){
            stepper.transmit_accelerometer_data = false;       
        }

    } /* parser check */
} /* G999() */

#endif