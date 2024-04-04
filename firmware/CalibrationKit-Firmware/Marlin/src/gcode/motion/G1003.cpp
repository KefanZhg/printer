#include "../gcode.h"
#include "../../module/stepper.h"

#ifdef ULNDO_STEPPER_SUPPORT

    // ULNDO_TODO: cleanup
    int32_t processed_commands = 0;
    constexpr int32_t ulendo_datasize = 450;
    unsigned char commandsBuff[450];
    char *token;  
    char step_dir_commands[1024];
    unsigned char char_recv;
    int dirs = 0;
    int data_idx = 0;

    /* 
    * Called by G-code G1003 takes commands in the format G1003 0,128,64,255
    * Parses the argument part of the string, and stores them in a buffer
    * Buffer is emptied by the execute command function
    */ 

    void GcodeSuite::G1003(){

        if (parser.string_arg){                        

            /**/
            strcpy(step_dir_commands, parser.string_arg);        

            // get the first token 
            token = strtok(step_dir_commands, ",");  

            // walk through other tokens 
            while( token != NULL ){ 

                // Convert String in 128 format to number
                dirs = atoi(token);

                // cast string into unsigned char
                char_recv = (char)dirs;            
                //SERIAL_ECHOLN(char_recv); // prints correctly

                // Push the stepper commands into the spare motion buffer
                commandsBuff[processed_commands] = char_recv;
                processed_commands = processed_commands + 1;    

                // If you have received all the commands, then add them to the buffer, HAL will execute
                if (processed_commands == ulendo_datasize) {                

                    // Wait for Interrupt to catch up to buffer input
                    // Allocate only two motion buffer due to the memory limitation           
                    while (stepper.data_flag[0] == false && stepper.data_flag[1] == false) {    
                        delayMicroseconds(1000);
                    }                

                    // if first memory buffer is empty store commands there else use second buffer
                    data_idx = 0;
                    if (stepper.data_flag[0] == true){
                        data_idx = 0;                    
                    }
                    else if (stepper.data_flag[1] == true){
                        data_idx = 1;                    
                    }

                    memcpy(stepper.motion_buffer[data_idx].buffer_commands, commandsBuff, sizeof(commandsBuff)); 
                    stepper.motion_buffer[data_idx].buffer_size = 450;

                    stepper.data_flag[data_idx] = false;
                    stepper.print_flag[data_idx] = true;

                    // Reset the buffer        
                    memset(commandsBuff, 0, 450); 

                    processed_commands = 0;
                }

                // get the next token from the string
                token = strtok(NULL, ",");  
                char_recv = 0;      
            }

        } /* parser check */
    } /* G1003() */

#endif /* ULNDO_STEPPER_SUPPORT */