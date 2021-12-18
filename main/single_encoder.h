#ifndef single_encoder_H_
#define single_encoder_H_

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string.h>
#include <iostream>
#include "driver/gpio.h"

#include "statemachine.h"
#include <tiny_encoder.h>

class Encoder : public TinyEncoder {
  gpio_num_t buttonPin = GPIO_NUM_NC;
  knob_t buttonStatus = knob_t::none;
  uint32_t buttonTimestamp = 0;

 public:
  // TODO : move it to .CPP
  Encoder(gpio_num_t leftPin_, gpio_num_t rightPin_, gpio_num_t buttonPin_):TinyEncoder(leftPin_,rightPin_){
    buttonPin  = buttonPin_;
    gpio_config_t tmp_io_conf{
        .pin_bit_mask = (1ULL<<buttonPin_),
        .mode         = GPIO_MODE_INPUT,
        .pull_up_en   = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,       
    };
    gpio_config(&tmp_io_conf);
  };

  void onCWclick(){ Device::process_input(knob_t::encCW); };
  void onCCWclick(){ Device::process_input(knob_t::encCCW); };

  /*
  void onCWclick(){
	  //motor->rotateCW();
    //Device::process_input(knob_t::anyOfLeftOrRightCapsEncoderRolled);
    //cout << "CW\n";
    //Lcd::threeStretchedDigits( (uint8_t) getClickCounter() , 0 , 55 );
    //Lcd2007::threeStretchedDigits( (uint8_t) getClickCounter() , 0 , 55 );
  };

  void onCCWclick(){
	  //motor->rotateCCW();
    //Device::process_input(knob_t::anyOfLeftOrRightCapsEncoderRolled);
    //cout << "CCW\n";
    //Lcd::threeStretchedDigits( (uint8_t) getClickCounter() , 0 , 55 );
    //Lcd2007::threeStretchedDigits( (uint8_t) getClickCounter() , 0 , 55 );
  };
  */

  void onButtonClick(){
    std::cout << " xXx \n";    
  };
  void poll() override;
  knob_t getButtonStatus(){ return buttonStatus; };

};

inline Encoder encoder(Settings::encLeft,Settings::encRight,Settings::encButton); 


/*
class MultiplierEncoder : public Encoder {
	static const uint32_t MINIMUM_VALUE = 2; // value=1
	static const uint32_t MAXIMUM_VALUE = 27; //23; // value=28657
	uint32_t multiplier_{MINIMUM_VALUE};
 	uint32_t recurrent_fibonacci(uint32_t n){
		if (n<=1){return n;}
		return ( fibonacci(n-1)+fibonacci(n-2) );
	}
	uint32_t fibonacci(uint32_t n){
	    uint32_t a=0,b=1;
	    if (n<=1){return n;}
	    for (uint32_t i=0;i<n-1;i++){
		    uint32_t c = a+b;
		    a=b;
		    b=c;
	    }
	    return b;
	}
 public:
  using Encoder::Encoder; // Inherit all constructors from base	
  virtual void onCWclick()=0;
  virtual void onCCWclick()=0;
  uint32_t value() { return fibonacci(multiplier_); };
  void incClick(){if(multiplier_<MAXIMUM_VALUE){multiplier_++;};};
  void decClick(){if(multiplier_>MINIMUM_VALUE){multiplier_--;};}; 
};

class CapsMultiplier : public MultiplierEncoder {
  knob_t buttonStatus = knob_t::none;
  uint32_t buttonTimestamp = 0;
  void onCWclick(){ Device::process_input(knob_t::capsMultEncCW); };
  void onCCWclick(){ Device::process_input(knob_t::capsMultEncCCW); };
 public:
  using MultiplierEncoder::MultiplierEncoder;
  void poll() override;
};

class AxisXMultiplier : public MultiplierEncoder {
  void onCWclick(){ Device::process_input(knob_t::axisMultEncCW); };
  void onCCWclick(){ Device::process_input(knob_t::axisMultEncCCW); };
 public:
  using MultiplierEncoder::MultiplierEncoder;
};

inline CapsMultiplier capsMultiplier(Settings_::capsMultiplierLpin,Settings_::capsMultiplierRpin);
inline AxisXMultiplier axisXMultiplier(Settings_::axisXMultiplierLpin,Settings_::axisXMultiplierRpin);
*/

#endif