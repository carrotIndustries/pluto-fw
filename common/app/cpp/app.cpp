//
// Created by lennart on 9/19/21.
//

#include "./app.h"
#define PRIV(a) ((priv_t*)((a)->priv))
#include "casio.hpp"
#include "framework/loop.hpp"
#include "framework/Event.hpp"

Casio casio;

static void coolfunction(uint8_t view, const app_t *app, svc_main_proc_event_t event) {
    if(event & SVC_MAIN_PROC_EVENT_TICK) {
        //casio.onTick();
        //trigger(TickEvent());
    }

    if(event & SVC_MAIN_PROC_EVENT_KEY_ENTER_LONG) {
        trigger(AlarmUpEvent());
    }
    if(event & SVC_MAIN_PROC_EVENT_KEY_ENTER) {
        trigger(AlarmDownEvent());
    }
    if(event & SVC_MAIN_PROC_EVENT_KEY_UP_LONG) {
        trigger(LightUpEvent());
    }
    if(event & SVC_MAIN_PROC_EVENT_KEY_UP) {
        trigger(LightDownEvent());
    }
    if(event & SVC_MAIN_PROC_EVENT_KEY_DOWN_LONG) {
        trigger(ModeUpEvent());
    }
    if(event & SVC_MAIN_PROC_EVENT_KEY_DOWN) {
        trigger(ModeDownEvent());
    }
    
//     auto str = new std::string();
//     str->append("oo");
// //    try{
// //        //throw (int) 5;
// //    }catch(int hey){
// //        int N = hey+6;
// //    }

//     hal_lcd_clear();
//     svc_lcd_puts(8, str->data());
//     // svc_lcd_puts(8, sv.data());
//     hal_lcd_seg_set(HAL_LCD_SEG_COLON, 1);
//     hal_lcd_seg_set_blink(HAL_LCD_SEG_COLON, true);
//     if(event) {
//         stringThing.toggle();
//     }
//     else if(event & SVC_MAIN_PROC_EVENT_KEY_UP) {
//         app_exit();
//     }

//     svc_lcd_puti(0, 2, 0);
//     svc_lcd_puti(2, 2, 0);
//     svc_lcd_puti(4, 2, 0);
//     delete str;
    //delete num;
}

static app_view_t view = {
        .main = coolfunction
};

static priv_t priv = {0};
//struct app_u {
//    char *name;
//    app_priv_t *priv;
//    void (*enter)(const app_t *app);
//    void (*leave)(const app_t *app);
//    app_view_t *views;
//    uint8_t n_views;
//};
extern "C" const app_t app_app_cpp = {
        .priv = (app_priv_t*)(&priv),
        .views = &view,
        .n_views = 1,
};