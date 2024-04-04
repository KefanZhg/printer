#include "../../inc/MarlinConfig.h"
#include "../../MarlinCore.h"
#include "../../lcd/marlinui.h"
#include "../../module/stepper.h"

#include "../gcode.h"

#if ENABLED(ULNDO_SINESWEEP_SUPPORT)

    /* 
    * Gets sine sweep parameters from Gcode, then enables control and data collection
    */

    void GcodeSuite::G998() {
    
        bool get_freq;
        bool get_amp;
        bool get_numPeaks;
        bool eStop;

        get_freq = parser.seenval('F');
        if (get_freq){
            stepper.input_freq = parser.floatval('F',3);
            SERIAL_ECHO(stepper.input_freq);       
        }
        get_amp = parser.seenval('A');
        if (get_amp){
            stepper.input_amp = parser.floatval('A',3);
            SERIAL_ECHO(stepper.input_amp);
        }
        get_numPeaks = parser.seenval('R');
        if (get_numPeaks){
            stepper.input_numPeaks = parser.floatval('R',3);
            SERIAL_ECHO(stepper.input_numPeaks);
        }
        eStop = parser.seenval('S');
        if (eStop == true){
            stepper.collect_stepperandaccel_data = false;
            SERIAL_ECHO("Data Collection Stopped");
        }  
        if (get_numPeaks == true && get_amp == true && get_freq == true){

            stepper.input_freq = (stepper.input_freq / 10000.0);
            stepper.time = 0;

            stepper.period = (1/(stepper.input_freq) * (stepper.input_numPeaks + stepper.move_up_interval));

            stepper.move_up_period = 1 / stepper.input_freq * (stepper.move_up_interval / 2);
            stepper.normal_period = 1 / stepper.input_freq * stepper.input_numPeaks;

            stepper.collect_stepperandaccel_data = true;
        }
    } /* G998() */
#endif /* ULNDO_SINESWEEP_SUPPORT */