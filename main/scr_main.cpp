#include "statemachine.h"
#include "local_settings.h"
#include "single_encoder.h"
#include "robot.h"

MainScreen::MainScreen(){
  Lcd::init();
  Lcd::clear();
}

void MainScreen::update_display(){
 	if (!Device::state->toBeRefreshed()){ return; }

 	static_assert(maximumPossibleItem >= screenSize - 1,"The list is too short.");
	
 static const char s0[] = "Direction:%s";
 static const char s1[] = "RealLength=%04d";
 static const char s2[] = "Multi=%04d"; 
 static const char s3[] = "Repeat   %03d";
 static const char s4[] = ">>   Run  <<";
 static const char s5[] = ">> Run x5 <<";;
 static const char s6[] = "Repeat   %03d";
 static const char s7[] = ">>   Run  <<";
 static const char s8[] = "ReverseLeftC"; // remove ?
 static const char s9[] = "ReverseRighC"; // remove ?
 static const char sa[] = "    Exit    ";

 	static const char* menuItems[maximumPossibleItem+1] = {s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,sa,};
 
	for (uint8_t i=0;i<screenSize;i++){
		switch (uppermostItem+i){
			case 0:
				if (Robot::testDirection == direction_t::CW){
					strcpy(Lcd::lazy,"Direction: CW");
				}else{
					strcpy(Lcd::lazy,"Direction:CCW");
				}				
			break;			
			case 1:sprintf(Lcd::lazy,menuItems[uppermostItem+i], Robot::testLength );
			break;
			case 2:sprintf(Lcd::lazy,menuItems[uppermostItem+i], Robot::testMulti );
			break;
			case 3:sprintf(Lcd::lazy,menuItems[uppermostItem+i], Robot::testRepeat );
			break;			
			/*
			case 5:sprintf(Lcd::lazy,s,Robot::getDepth());
			break;
			case 6:sprintf(Lcd::lazy,s,Robot::getRepeat());
			break;
			case 7:sprintf(Lcd::lazy,s,4);
			break;
			*/
			default: strcpy(Lcd::lazy,menuItems[uppermostItem+i]);
			break;
		} // switch

    	Lcd::gotoXY(0,i);
		Lcd::string(Lcd::lazy,(i==(highlitedItem-uppermostItem)));
 
	}// for
	Device::state->refresh(false);
}

void MainScreen::process_input(knob_t knob){
 
 Device::state->refresh(true);

 if (encoder.getButtonStatus()==knob_t::encPressed){
	switch(knob){
 	case knob_t::encCW:
		switch (highlitedItem){
			case 1: // realLength
				Robot::testLength = Robot::testLength+Robot::testMulti;
			break;
			case 2: // Multi
				Robot::testMulti = Robot::testMulti+1;
			break;
			case 3: // Repeat
				Robot::testRepeat = Robot::testRepeat+1;
			break;						
			default:
			break;
		}
    break;
	case knob_t::encCCW:
		switch (highlitedItem){
			case 1: // realLength
				if (Robot::testLength >= Robot::testMulti){
					Robot::testLength = Robot::testLength-Robot::testMulti;
				}
			break;
			case 2: // Multi
				if (Robot::testMulti>0){
					Robot::testMulti = Robot::testMulti-1;
				}
			break;
			case 3: // Repeat
				if (Robot::testRepeat>0){
					Robot::testRepeat = Robot::testRepeat-1;
				}
			break;
			default:
			break;
		}
    break;
	default:
	break;
	}

	return;
 } //encPressed	
 
 switch(knob){
 	case knob_t::encCW:
	if(highlitedItem - uppermostItem + 1 < screenSize){
		if(highlitedItem < maximumPossibleItem){highlitedItem++;} // insurance overkill
		}else{ // scroll up
		if(highlitedItem < maximumPossibleItem){
			uppermostItem++;
			highlitedItem++;
			}else{
			//uppermostItem=0;
			//highlitedItem=0;
		}
	}
	break;
	case knob_t::encCCW:
	if(highlitedItem > uppermostItem){
		if(highlitedItem > 0){highlitedItem--;} // insurance overkill
		}else{ // scroll down
		if (highlitedItem > 0){
			uppermostItem--;
			highlitedItem--;
			}else{
			//highlitedItem = maximumPossibleItem;
			//uppermostItem = highlitedItem - screenSize + 1;
		}
	}
	break;
	case knob_t::encReleased:
 		switch (highlitedItem){
		case 0: // direction
		 Robot::testDirection = (Robot::testDirection==direction_t::CW)?direction_t::CCW:direction_t::CW;
		break;		
		case 4:
		 runPrototype();
		 Lcd::clear();
		break;
		case 5:
		 runPrototypeX5();
		 Lcd::clear(); 
		break;

		default:break; 
	   }
	break;
  default:

  break;
 }
 	

}

void MainScreen::runPrototype(){

  for(uint8_t i=0;i<Robot::testRepeat;i++){
   Lcd::init(); // just in case	
   Lcd::clear();
 
   Lcd::stretchedString(0,0,"*Runing*");
   sprintf(Lcd::lazy,"L:%06u", Robot::testLength );
   Lcd::stretchedString(0,4, Lcd::lazy);

   sprintf(Lcd::lazy,"R:%02uof%02u", Robot::testRepeat-i,Robot::testRepeat  );
   Lcd::stretchedString(0,6, Lcd::lazy);     
  
   axisXMotor.blockingRotate( Robot::testLength, Robot::testDirection );
  }
  
  Lcd::clear();
  Lcd::string("Done.");
  vTaskDelay(pdMS_TO_TICKS(2000));

}

void MainScreen::runPrototypeX5(){

   for(uint8_t i=0;i<Robot::testRepeat;i++){
     Lcd::init(); // just in case	
     Lcd::clear();
 
     Lcd::stretchedString(0,0,"Run Forward");
     sprintf(Lcd::lazy,"L:%06u", Robot::testLength );
     Lcd::stretchedString(0,4, Lcd::lazy);

     sprintf(Lcd::lazy,"R:%02uof%02u", Robot::testRepeat-i,Robot::testRepeat  );
     Lcd::stretchedString(0,6, Lcd::lazy);     
  
     axisXMotor.blockingRotate( Robot::testLength, direction_t::CW );
     vTaskDelay(pdMS_TO_TICKS(1000));
   }

   Lcd::init(); // just in case	
   Lcd::clear();
   Lcd::stretchedString(0,0,"Run Backward");
   axisXMotor.blockingRotate( Robot::testLength * 5, direction_t::CCW );
 
   Lcd::clear();
   Lcd::string("Done.");
   vTaskDelay(pdMS_TO_TICKS(2000));
}
