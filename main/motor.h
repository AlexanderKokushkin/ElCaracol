#ifndef motor_H_
#define motor_H_
#include "driver/gpio.h"
#include "local_settings.h"

enum class direction_t : uint8_t {CW=0,CCW=1};

class SomeMotor{
    gpio_num_t dir = GPIO_NUM_NC;
    gpio_num_t pull = GPIO_NUM_NC;

	direction_t direction = direction_t::CW;

	uint16_t stepsPending = 0; // amount of steps in queue
	enum class phase_t { dir,pullHigh,pullLow };
	phase_t phase = phase_t::dir;
	
	int32_t stepCounter = 0; // "reversed" is ignored here
	uint32_t acceleration = 0;
	uint32_t accelerationThreshold = 500;
	
	uint32_t deceleration = 0;
	uint32_t decelerationThreshold = 1000;
	
	uint32_t pollTimestamp=0;
	uint32_t pollIntervalMKS=25;

	uint32_t accelTimestamp=0;
	static const uint32_t DFLTaccelIntervalMKS=500;
	uint32_t accelIntervalMKS=DFLTaccelIntervalMKS;
	
	static const uint32_t POLL_INTERVAL_MAX = 500;
	static const uint32_t POLL_INTERVAL_MIN = 2 ;
 public:
	SomeMotor(gpio_num_t dirPin_, gpio_num_t pullPin_);
	void setPollIntervalMKS(uint32_t value);
	uint32_t getPollIntervalMKS(){return pollIntervalMKS;}
	void rotateCW();
	void rotateCCW();
	
	void blockingRotate(uint32_t value,direction_t dir_);
	
	void smoothStop(){stepsPending=50;}
			
	int32_t getStepCounter(){return stepCounter;}
	void resetStepCounter(){stepCounter=0;}
	void poll();
	void setDecelerationThreshold(uint32_t v){decelerationThreshold=v;}
	void setAccelerationThreshold(uint32_t v){accelerationThreshold=v;}
};

inline SomeMotor axisXMotor(Settings::dir,Settings::pull);

#endif