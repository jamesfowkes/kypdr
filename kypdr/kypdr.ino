















/*
kypdr
Created on 2018-11-15
By the Arduino Description Language tool.
 

        Declares required ADL files for the kypdr board
    
*/





#include <stdint.h>

#include "adl.h"






#include "binary-output.h"





static BinaryOutput s_binary_output = BinaryOutput(5, 4, 3, PIN_NOT_USED, PIN_NOT_USED, PIN_NOT_USED, PIN_NOT_USED, PIN_NOT_USED);


static DeviceBase * s_devices[] = 
{
    
    &s_binary_output
    
    
};



static ParameterBase * s_params[] = 
{
    
};






int handle_device1_command(char const * const command, char * reply)
{
    return s_binary_output.command_handler(command, reply);
}


static COMMAND_HANDLER adl_devices[] = {
    
    handle_device1_command,
    
};

COMMAND_HANDLER& adl_get_device_cmd_handler(DEVICE_ADDRESS address)
{
    return adl_devices[address-1];
}

DeviceBase& adl_get_device(DEVICE_ADDRESS address)
{
    return *s_devices[address-1];
}



static COMMAND_HANDLER adl_params[] = {
    
};

COMMAND_HANDLER& adl_get_param_cmd_handler(PARAM_ADDRESS address)
{
    return adl_params[address-1];
}

ParameterBase& adl_get_param(PARAM_ADDRESS address)
{
    return *s_params[address-1];
}



void adl_board_send(char * to_send)
{
    Serial.println(to_send);
}


void setup()
{
    adl_on_setup_start();

    Serial.begin(115200);
    
    adl_nonvolatile_setup();

    adl_logging_setup(Serial);

    
    // Setup for Binary Output
    s_binary_output.setup();
    // END Binary Output setup
    

    

    adl_custom_setup(s_devices, ADL_DEVICE_COUNT, s_params, ADL_PARAM_COUNT);

    adl_on_setup_complete();
    
    if (0)
    {
        adl_delay_start( 0 );
    }
}



void loop()
{
    adl_handle_any_pending_commands();
    adl_service_timer();
    adl_custom_loop(s_devices, ADL_DEVICE_COUNT, s_params, ADL_PARAM_COUNT);
}



        void serialEvent()
        {
            while (Serial.available())
            {
                adl_add_char((char)Serial.read());
            }
        }



