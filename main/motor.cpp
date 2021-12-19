#include "motor.h"

SomeMotor::SomeMotor(gpio_num_t dirPin_, gpio_num_t pullPin_){

	dir  = dirPin_;
	pull = pullPin_;
    gpio_config_t tmp_io_conf{
        .pin_bit_mask = ((1ULL<<dir) | (1ULL<<pull)),
        .mode         = GPIO_MODE_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,       
    };
    gpio_config(&tmp_io_conf);
}



void SomeMotor::blockingRotate(uint32_t value,direction_t dir_){ 
 // todo : BoilerPlate	
 if (stepsPending){  smoothStop();  return; } // that's should not be this way
 phase = phase_t::dir;
 direction = dir_;
 stepsPending = value;
 
 acceleration = accelerationThreshold;
 accelTimestamp = esp_timer_get_time()+accelIntervalMKS;
 accelIntervalMKS = DFLTaccelIntervalMKS;
 pollTimestamp = esp_timer_get_time() + pollIntervalMKS + POLL_INTERVAL_MIN*acceleration;
 deceleration = 0;
  
 while(stepsPending){
   poll();
 }
}

/*
void SomeMotor::rotateCW(){
 if ((stepsPending)and(direction!=direction_t::CW)){
  smoothStop(); 
  return; 
 }  	
 phase = phase_t::dir;
 direction = direction_t::CW;
 stepsPending = multiplier->value(); 
 
 acceleration = accelerationThreshold;
 accelTimestamp = esp_timer_get_time()+accelIntervalMKS;
 accelIntervalMKS = DFLTaccelIntervalMKS;
 pollTimestamp = esp_timer_get_time() + pollIntervalMKS + POLL_INTERVAL_MIN*acceleration;
 //pollTimestamp = esp_timer_get_time()+pollIntervalMKS; // i.e. immediately
 deceleration = 0;
}

void SomeMotor::rotateCCW(){
 if ((stepsPending)and(direction!=direction_t::CCW)){
  smoothStop();
  return;
 }	
 phase = phase_t::dir;	
 direction = direction_t::CCW;
 stepsPending = multiplier->value();

 acceleration = accelerationThreshold;
 accelTimestamp = esp_timer_get_time()+accelIntervalMKS;
 accelIntervalMKS = DFLTaccelIntervalMKS;
 pollTimestamp = esp_timer_get_time() + pollIntervalMKS + POLL_INTERVAL_MIN*acceleration;
 //pollTimestamp = esp_timer_get_time()+pollIntervalMKS; // i.e. immediately
 deceleration = 0;
}
*/

 void SomeMotor::poll(){

 if (!stepsPending){ return; }
	 
 if( esp_timer_get_time() - accelTimestamp > accelIntervalMKS){
	 accelTimestamp = esp_timer_get_time();
	 if(acceleration){
		 acceleration--;
	     accelIntervalMKS += 5; // debug??
	 };
    // kludge 
    if (stepsPending<decelerationThreshold){ deceleration++; }
 }  	
 
 
 if ( esp_timer_get_time() - pollTimestamp < pollIntervalMKS + 
                                 POLL_INTERVAL_MIN*acceleration+
			                     POLL_INTERVAL_MIN*deceleration ) { return;}

 //if ((stepsPending%10==0)and(acceleration)){ acceleration--;}
 //if ((stepsPending%(ACCELERATION-acceleration)==0)and(acceleration)){ acceleration--;}

 pollTimestamp = esp_timer_get_time();
 switch (phase){
	   case phase_t::dir:
	   {
		if(direction==direction_t::CCW){ // kludge
		 //*dirPinPort &=	~dirPinMask; // clear bit
		 gpio_set_level(dir,0);
		}else{
		 //*dirPinPort |= dirPinMask; // set bit
		 gpio_set_level(dir,1);
		}
	    phase = phase_t::pullHigh;
		}
	   break;
	   case phase_t::pullHigh:
		//*pullPinPort |= pullPinMask; // set bit
		gpio_set_level(pull,1);
	    phase = phase_t::pullLow;
	   break;
	   case phase_t::pullLow:
		//*pullPinPort &= ~pullPinMask; // clear bit
		gpio_set_level(pull,0);
	    phase = phase_t::pullHigh;
	    stepsPending--;
		if (direction==direction_t::CW){
		 stepCounter++;	
		}else{
		 stepCounter--;	
		}
	   break;	   
 }
}

void SomeMotor::setPollIntervalMKS(uint32_t value){
	if (value > POLL_INTERVAL_MAX){return;}
	if (value < POLL_INTERVAL_MIN){return;}	
	pollIntervalMKS=value;
};

