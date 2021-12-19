#ifndef ROBOT_H_
#define ROBOT_H_
#include "motor.h"
// TODO : use it

class Robot{
  inline static uint16_t length = 50000;
  inline static uint16_t depth = 0;
  inline static uint8_t repeat = 0;
 public:
  static uint16_t getLength(){return length;}
  static void setLength(uint16_t value){ length = value;}
  static uint16_t getDepth(){return depth;}
  static void setDepth(uint16_t value){ depth = value;} 	
  static uint8_t getRepeat(){return repeat;}
  static void setRepeat(uint16_t value){ repeat = value;}	

  // temporary, for tests
  inline static direction_t testDirection = direction_t::CW;
  inline static uint16_t testLength = 10000;
  inline static uint16_t testMulti = 10;
  inline static uint8_t testRepeat = 1; 
};

#endif