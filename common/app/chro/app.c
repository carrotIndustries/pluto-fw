#include "common/app/app.h"
#include "common/app/apps.h"
#include "common/svc/svc.h"
#include "common/hal/hal.h"
#include "common/hal/lcd_segments.h"
#include <string.h>

typedef struct {
    APP_PRIV_COMMON
    uint8_t lcd_needs_clear;
    svc_chro_t ch_last;
    svc_chro_state_t st_last;
} priv_t;

#define PRIV(a) ((priv_t*)((a)->priv))

static void main(uint8_t view, const app_t *app, svc_main_proc_event_t event) {
    hal_lcd_set_mode(HAL_LCD_MODE_IMMEDIATE);
    svc_chro_t ch;
    svc_chro_t *ch_last = &(PRIV(app)->ch_last);
    svc_chro_get(0, &ch);
    svc_chro_convert(&ch);

    svc_chro_state_t st = svc_chro_get_state(0);

    if(svc_lcd_get_force_redraw() || (st != PRIV(app)->st_last)) {
        PRIV(app)->lcd_needs_clear = 1;
    }
    if(PRIV(app)->lcd_needs_clear) {
        hal_lcd_clear();
        memset(ch_last, 0xff, sizeof(svc_chro_t));
        memset(&(PRIV(app)->st_last), 0xff, sizeof(svc_chro_state_t));
        PRIV(app)->lcd_needs_clear = 0;
    }

    /* key handling **********************************************************/
    if(event & (SVC_MAIN_PROC_EVENT_KEY_ANY | SVC_MAIN_PROC_EVENT_KEY_ANY_LONG)) {
        if(event & SVC_MAIN_PROC_EVENT_KEY_ENTER) {
            switch(st) {
            case SVC_CHRO_STATE_RUN :
                svc_chro_hold(0);
                break;

            case SVC_CHRO_STATE_HOLD :
            case SVC_CHRO_STATE_STOP :
                svc_chro_start(0);
                break;
            }
        }
        else if(event & SVC_MAIN_PROC_EVENT_KEY_DOWN) {
            switch(st) {
            case SVC_CHRO_STATE_RUN :
            case SVC_CHRO_STATE_HOLD :
                svc_chro_stop(0);
                break;

            case SVC_CHRO_STATE_STOP :
                svc_chro_reset(0);
                break;
            }
        }
        else if(event & SVC_MAIN_PROC_EVENT_KEY_DOWN_LONG) {
            svc_chro_stop(0);
            svc_chro_reset(0);
        }
        else if(event & (SVC_MAIN_PROC_EVENT_KEY_ENTER_LONG | SVC_MAIN_PROC_EVENT_KEY_UP)) {
            app_exit();
        }
    }

    /* refresh display *******************************************************/
    hal_lcd_seg_set(HAL_LCD_SEG_COLON, 1);
    hal_lcd_seg_set_blink(HAL_LCD_SEG_COLON, st==SVC_CHRO_STATE_RUN);

    if(ch.min != ch_last->min)
        svc_lcd_puti_fast(0, 2, ch.min);
    if(ch.sec != ch_last->sec)
        svc_lcd_puti_fast(2, 2, ch.sec);
    if(ch.subsec != ch_last->subsec)
        svc_lcd_puti_fast(4, 2, ch.subsec);

    if(st != PRIV(app)->st_last) {
        switch(st) {
        case SVC_CHRO_STATE_RUN :
            svc_lcd_puts(8, "ru");
            break;

        case SVC_CHRO_STATE_HOLD :
            svc_lcd_puts(8, "hl");
            break;

        case SVC_CHRO_STATE_STOP :
            svc_lcd_puts(8, "st");
            break;
        }
    }

    *ch_last = ch;
    PRIV(app)->st_last = st;
}

void main_enter(uint8_t view, const app_t *app) {
    PRIV(app)->lcd_needs_clear = 1;
}

static app_view_t view = {
    .main = main,
    .enter = main_enter
};

static priv_t priv = {0};

const app_t app_app_chro = {
    .n_views = 1,
    .priv = (app_priv_t*)(&priv),
    .views = &view
};
