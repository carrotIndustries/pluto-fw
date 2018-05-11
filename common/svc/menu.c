#include "menu.h"
#include "lcd.h"
#include "util.h"
#include "common/app/apps.h"
#include "common/hal/hal.h"
#include "platform.h"

extern const uint8_t app_time_display_view;
static const uint8_t time_to_return_home_min = 5;
static uint8_t SECTION_INFOMEM time_to_return_home = 42;

void svc_menu_run(const svc_menu_t *menu, svc_menu_state_t *state, svc_main_proc_event_t event) {
	uint8_t *item_current = &(state->item_current);
	hal_lcd_clear();
	if(state->adj_mode == 0) {
		if(event & SVC_MAIN_PROC_EVENT_KEY_DOWN) {
			INC_MOD(*item_current, menu->n_items);
		}
		else if(event & SVC_MAIN_PROC_EVENT_KEY_UP) {
			DEC_MOD(*item_current, menu->n_items);
		}
		else if(event & SVC_MAIN_PROC_EVENT_KEY_ENTER_LONG) {
			if(menu->handler_exit) {
				menu->handler_exit();
			}
		}
		else if(event & SVC_MAIN_PROC_EVENT_KEY_ENTER) {
			svc_menu_item_unknown_t *it_u = menu->items[*item_current];
			switch(it_u->type) {
				case SVC_MENU_ITEM_T_TEXT : {
					svc_menu_item_text_t *it = (void*)it_u;
					if(it->handler) {
						it->handler(it->user_data);
					}
				} break;

				case SVC_MENU_ITEM_T_CHOICE : {
					svc_menu_item_choice_t *it = (void*)it_u;
					uint8_t c = it->handler_get(it->user_data);
					INC_MOD(c, it->n_choices);
					if(it->handler_set) {
						it->handler_set(c, it->user_data);
					}
				} break;

				case SVC_MENU_ITEM_T_ADJ : {
					svc_menu_item_adj_t *it = (void*)it_u;
					state->adj_mode = 1;
					state->adj_digit = it->digits-1;
					if(it->handler_enter) {
						it->handler_enter(it->user_data);
					}
				} break;
			}
		}
		svc_menu_item_unknown_t *it_u = menu->items[*item_current];

		if(menu->header) {
			svc_lcd_puts(menu->header_pos, menu->header);
		}

		switch(it_u->type) {
			case SVC_MENU_ITEM_T_TEXT : {
				svc_menu_item_text_t *it = (void*)it_u;
				svc_lcd_puts(0, it->text);

			} break;
			case SVC_MENU_ITEM_T_CHOICE : {
				svc_menu_item_choice_t *it = (void*)it_u;
				svc_lcd_puts(0, it->text);
				uint8_t c = it->handler_get(it->user_data);
				if(it->choices[0][0] == 0) {
					svc_lcd_puti(it->choice_pos, 2, c);
				}
				else {
					svc_lcd_puts(it->choice_pos, it->choices[c]);
				}
			} break;
			case SVC_MENU_ITEM_T_ADJ : {
				svc_menu_item_adj_t *it = (void*)it_u;
				svc_lcd_puts(0, it->text);
			} break;
		}
		if(it_u->handler_draw) {
			it_u->handler_draw(state, it_u, it_u->user_data);
		}
	}
	else {
		svc_menu_item_adj_t *it = (void*)menu->items[*item_current];
		if(it->type != SVC_MENU_ITEM_T_ADJ) {
			state->adj_mode = 0;
			return;
		}

		svc_lcd_puts(8, it->header);
		if(event & SVC_MAIN_PROC_EVENT_KEY_UP) {
			svc_lcd_blink_disable();
			it->handler_set(it->digits-state->adj_digit-1, 1, it->user_data);
		}
		else if(event & SVC_MAIN_PROC_EVENT_KEY_DOWN) {
			svc_lcd_blink_disable();
			it->handler_set(it->digits-state->adj_digit-1, -1, it->user_data);
		}

		if(event & SVC_MAIN_PROC_EVENT_KEY_ENTER_LONG) {
			state->adj_mode = 0;
			if(it->handler_leave) {
				it->handler_leave(it->user_data);
			}
		}

		svc_lcd_puti(0, it->digits, it->handler_get(it->user_data));
		if(event & SVC_MAIN_PROC_EVENT_KEY_ENTER) {
			INC_MOD(state->adj_digit, it->digits);
		}
		if(state->adj_digit < it->digits) {
			hal_lcd_dig_set_blink_mask(1<<state->adj_digit);
		}
	}
}

uint8_t svc_menu_timetohome_get(void) {
	return time_to_return_home;
}

void svc_menu_timetohome_set(uint8_t time) {
	if(time < time_to_return_home_min) {
		time_to_return_home = 0; /* return to home is disabled */
	}
	else {
		time_to_return_home = time;
	}
}

uint8_t svc_menu_timetohome_min_get(void) {
	return time_to_return_home_min;
}

static uint8_t time_idle;
void svc_menu_process_timetohome(uint8_t keypress) {
	if(!time_to_return_home) {
		return; /* return to home is disabled */
	}

	if(keypress) {
		time_idle = 0;
	}
	else {
		time_idle += 1;
	}

	if(time_idle == time_to_return_home) {
		if(app_current != &app_app_time) {
			app_launch(&app_app_time);
		}
		else if(app_current->priv->view_current != app_time_display_view) {
			app_set_view(app_current, app_time_display_view);
		}
	}
}
