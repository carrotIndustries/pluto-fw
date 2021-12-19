#include "casio.hpp"

Casio::Casio(): counter(0){

}

void Casio::onTick(){
    counter = (counter + 1) % 60;
    hal_lcd_clear();
    svc_lcd_puts(8, "ou");
    // svc_lcd_puts(8, sv.data());
    hal_lcd_seg_set(HAL_LCD_SEG_COLON, 1);
    hal_lcd_seg_set_blink(HAL_LCD_SEG_COLON, true);
    svc_lcd_puti(0, 2, counter);
    svc_lcd_puti(2, 2, counter);
    svc_lcd_puti(4, 2, counter);
    
}

void Casio::onShortPress(const Button& b){
    
}

void Casio::onLongPress(const Button& b){
    Beeper::beep();
}

void Casio::onAuxTimer(){

}

Casio::~Casio(){

}