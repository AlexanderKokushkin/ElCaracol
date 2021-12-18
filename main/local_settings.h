#ifndef local_settings_H_
#define local_settings_H_

#include <lcd_st7565.h>
class Settings{
    public:
        static constexpr gpio_num_t lcd_rst = GPIO_NUM_32; // RSE
        static constexpr gpio_num_t lcd_rs  = GPIO_NUM_33; // RS/A0/RegisterSelect/DataOrCtrl 
        static constexpr gpio_num_t lcd_scl = GPIO_NUM_25; // SCL/clk~clock
        static constexpr gpio_num_t lcd_si  = GPIO_NUM_26; // SI/sid~SerialInputData

        static constexpr gpio_num_t encLeft   = GPIO_NUM_34;
        static constexpr gpio_num_t encRight  = GPIO_NUM_35;
        static constexpr gpio_num_t encButton = GPIO_NUM_39;
        
        static constexpr gpio_num_t dir  = GPIO_NUM_27;
        static constexpr gpio_num_t pull = GPIO_NUM_14;
};
using Lcd = Lcd7565_T<Settings>;

#endif