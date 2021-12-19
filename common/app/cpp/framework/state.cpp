#include "state.hpp"
#include <bitset>

void State::realize(){
    hal_lcd_clear();

    if(pm.has_value()){
        hal_lcd_seg_set(HAL_LCD_SEG_PM, pm.value() ? 1 : 0);
    }
    if(twentyfour.has_value()){
        hal_lcd_seg_set(HAL_LCD_SEG_24H, twentyfour.value() ? 1 : 0);
    }
    if(alarm.has_value()){
        hal_lcd_seg_set(HAL_LCD_SEG_BARS, alarm.value() ? 1 : 0);
    }
    if(bell.has_value()){
        hal_lcd_seg_set(HAL_LCD_SEG_BELL, bell.value() ? 1 : 0);
    }
    if(lap.has_value()){
        hal_lcd_seg_set(HAL_LCD_SEG_LAP, lap.value() ? 1 : 0);
    }
    if(colon.has_value()){
        hal_lcd_seg_set(HAL_LCD_SEG_COLON, colon.value() ? 1 : 0);
    }
    if(dot.has_value()){
        // noop
    }
    if(backlight.has_value()){
        hal_backlight_set(backlight.value() ? 10 : 0);
    }
    if(beeper.has_value()){
        hal_beep(beeper.value() ? 700 : 0);
    }
    for(int i = 0 ; i<10;i++){
        if(text[i].has_value()){
            svc_lcd_putc(i, text[i].value());
        }
    }

    // svc_lcd_puts(9, "u");
    // //svc_lcd_putc(8, '4');
    // // svc_lcd_puts(8, sv.data());
    
    // svc_lcd_putc(8, text[0]);
    // svc_lcd_putsn(0, 10, text);
}

void State::layer(const State& other){
    if(other.pm.has_value()){
        pm = other.pm;
    }
    if(other.twentyfour.has_value()){
        twentyfour = other.twentyfour;
    }
    if(other.alarm.has_value()){
        alarm = other.alarm;
    }
    if(other.bell.has_value()){
        bell = other.bell;
    }
    if(other.lap.has_value()){
        lap = other.lap;
    }
    if(other.colon.has_value()){
        colon = other.colon;
    }
    if(other.dot.has_value()){
        dot = other.dot;
    }
    if(other.backlight.has_value()){
        backlight = other.backlight;
    }
    if(other.beeper.has_value()){
        beeper = other.beeper;
    }
    for(int i = 0 ; i<10;i++){
        if(other.text[i].has_value()){
            text[i] = other.text[i];
        }
    }
}