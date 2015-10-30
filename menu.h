#if !defined _MENU_INCLUDED
#define _MENU_INCLUDED

#include "common.h"

typedef void (* menu_item_t)(void);

enum menu_mode_e
{
	MENU_MODE_START = 0,
    /*
	MENU_MODE_CONTROLS,
	MENU_MODE_USART,
	MENU_MODE_VERSION,
    */
	MENU_MODE_COUNT
};

enum menu_item_e
{
	//MENU_ITEM_COUNT = 15
	MENU_ITEM_COUNT = 1
};

void menu_init(void);
void menu_doitem(void);
uint8_t menu_getitem(void);
uint8_t menu_get_mode(void);
void menu_mode_next(void);
void menu_mode_prev(void);
void menu_item_next(void);
void menu_item_prev(void);

extern menu_item_t menu_items[MENU_MODE_COUNT][MENU_ITEM_COUNT];

extern uint8_t	g_menu_mode;
extern uint8_t	g_menu_item[MENU_MODE_COUNT];

extern uint8_t	g_menu_changed;

#endif /* _MENU_INCLUDED */
