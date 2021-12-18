#include "single_encoder.h"

void Encoder::poll(){
    TinyEncoder::poll();

    if ((buttonStatus == knob_t::none)and(millis() - buttonTimestamp > 500)and(gpio_get_level(buttonPin)==0)){
	  buttonTimestamp = millis();
	  buttonStatus = knob_t::encPressed;
    }

    if ((buttonStatus == knob_t::encPressed)and(millis() - buttonTimestamp > 50)and(gpio_get_level(buttonPin)!=0)){
	  buttonTimestamp = millis();
	  buttonStatus = knob_t::none;
	  Device::process_input(knob_t::encReleased);
    }

};
