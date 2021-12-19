#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include <generic_tools.h>
#include "local_settings.h"

 // TODO move it to single_encoder?
 enum class knob_t : uint8_t { none=0, encCW=1, encCCW=2, encPressed=3, encReleased=4 };   
 
 class State {
   bool toBeRefreshed_ = true;	 
  public:
   virtual void update_display()=0;
   virtual void poll()=0;
   virtual void process_input(knob_t knob)=0;
   virtual ~State(){};
   void refresh(bool value){toBeRefreshed_ = value;};
   bool toBeRefreshed(){return toBeRefreshed_;};	   
 };
 	 	
 class MainScreen : public State {
   bool toBeRefreshed_ = false;

   uint8_t highlitedItem = 0;
   uint8_t uppermostItem = 0;
   static const uint8_t screenSize = 8;
   static const uint8_t maximumPossibleItem = 10; // != sizeOfArray (it's less)

  public:
   void update_display() override;
   void poll() override {};
   void process_input(knob_t knob) override;
   MainScreen();
   ~MainScreen(){};
   void runPrototype();
   void runPrototypeX5();
 };
 /*
 class SetupMenu : public State {
   uint8_t highlitedItem = 0;
   uint8_t uppermostItem = 0;
   static const uint8_t screenSize = 8;
   static const uint8_t maximumPossibleItem = 10; // != sizeOfArray (it's less)
  public:
   void update_display() override;
   void poll() override {};
   void process_input(knob_t knob) override;
   SetupMenu();
   ~SetupMenu(){};
   void runPrototype();
 };
*/

 class Device {
   inline static uint32_t updateTimestamp{0};
   static const uint32_t UPDATE_INTERVAL = 50; //200;	 
  public:
   inline static State* state = nullptr;
   static void init(){
 	   Device::state = new MainScreen();
   }
   static void poll(){
	   if (state == nullptr ){return;};
	   state->poll();
	   if (millis()-updateTimestamp<UPDATE_INTERVAL){return;};
	   updateTimestamp = millis();
	   state->update_display();
   }
   static void process_input(knob_t knob){
	   if (state == nullptr ){return;};
	   state->process_input( knob );
   };
 };

#endif