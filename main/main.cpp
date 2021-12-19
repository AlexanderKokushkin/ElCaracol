#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string.h>
#include <iostream>
#include "driver/gpio.h"

#include <generic_tools.h>
#include "local_settings.h"
#include <tiny_encoder.h>
#include "statemachine.h"
#include "single_encoder.h"
#include "motor.h"

using std::cout;
uint32_t currentMills = 0;

static void superLoop(void* v){
    for(;;){
        //vTaskDelay(2 / portTICK_RATE_MS);
        encoder.poll();
        Device::poll();
        axisXMotor.poll(); 
    }
    vTaskDelete(NULL);
}

extern "C" void app_main(){
   
  Lcd::init();
  Lcd::clear();
  Lcd::string("ElCaracol v0.0");
  vTaskDelay(pdMS_TO_TICKS(2000));
  Device::init();


   /*
    gpio_config_t tmp_io_conf{
        .pin_bit_mask = ((1ULL<<dir) | (1ULL<<pull)),
        .mode         = GPIO_MODE_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,       
    };
    gpio_config(&tmp_io_conf);

    gpio_set_level(dir,1);
    for (uint16_t j=0; j<5; ++j){
     for (uint16_t i = 0; i < 200; ++i){
        gpio_set_level(pull,1);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level(pull,0);
        vTaskDelay(pdMS_TO_TICKS(10));
     }
     vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
    gpio_set_level(dir,0);
    for (uint16_t i = 0; i < 1000; ++i){
        gpio_set_level(pull,1);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level(pull,0);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    */

   xTaskCreatePinnedToCore(superLoop, "superLoop", 2048, nullptr, tskIDLE_PRIORITY, nullptr, tskNO_AFFINITY);
   //while(1){ vTaskDelay(pdMS_TO_TICKS(1000));  }
}

