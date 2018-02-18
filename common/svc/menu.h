#pragma once
#include <stdint.h>
#include "main.h"

typedef enum {
	SVC_MENU_ITEM_T_TEXT,
	SVC_MENU_ITEM_T_CHOICE,
	SVC_MENU_ITEM_T_ADJ,
} svc_menu_item_type_t;

typedef struct {
	uint8_t item_current;
	uint8_t adj_mode;
	uint8_t adj_digit;
} svc_menu_state_t;

struct svc_menu_item_unknown_u;

typedef struct svc_menu_item_unknown_u svc_menu_item_unknown_t;

#define SVC_MENU_ITEM_COMMON \
	svc_menu_item_type_t type; \
	void *user_data; \
	void (*handler_draw)(svc_menu_state_t *state, svc_menu_item_unknown_t *item, void *user_data);

struct svc_menu_item_unknown_u {
	SVC_MENU_ITEM_COMMON
};

typedef struct {
	SVC_MENU_ITEM_COMMON
	char *text;
	void (*handler)(void *user_data);
} svc_menu_item_text_t;

typedef struct {
	SVC_MENU_ITEM_COMMON
	char *text;
	void (*handler_set)(uint8_t choice, void *user_data);
	uint8_t (*handler_get)(void *user_data);
	uint8_t n_choices;
	uint8_t choice_pos;
	char *choices[];
} svc_menu_item_choice_t;

typedef struct {
	SVC_MENU_ITEM_COMMON
	char *text;
	char *header;
	void (*handler_set)(uint8_t dig, int8_t dir, void *user_data);
	int32_t (*handler_get)(void *user_data);
	void (*handler_enter)(void *user_data);
	void (*handler_leave)(void *user_data);
	uint8_t digits;
} svc_menu_item_adj_t;

typedef struct {
	uint8_t n_items;
	svc_menu_item_unknown_t **items;
	void (*handler_exit)(void);
	char *header;
	uint8_t header_pos;
} svc_menu_t;

void svc_menu_run(const svc_menu_t *menu, svc_menu_state_t *state, svc_main_proc_event_t event);
uint8_t svc_menu_timetohome_get(void);
void svc_menu_timetohome_set(uint8_t time);
uint8_t svc_menu_timetohome_min_get(void);
void svc_menu_process_timetohome(uint8_t keypress);
