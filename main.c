#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "usart.h"
#include "lcd.h"
#include "beep.h"
#include "timer.h"
#include "shift_out.h"
#include "kbd.h"
#include "sensor.h"
#include "modbus.h"
#include "menu.h"
#include "menu_items.h"

modbus_cmd_s	cmd;
uint8_t			msg[MODBUS_MAX_MSG_LENGTH];
result_e		res;

uint8_t	g_usart0_modbus_id;
long	g_usart0_baudrate;
long	g_usart1_baudrate;

extern	int	g_usart1_last_error;
extern  int	g_last_nak;

void process_usart(void);
void process_cmd(modbus_cmd_s * /* cmd */);

void reset_settings(void);

int m0 = 0;
int m1 = 0;

int idx = 0;

int main(void)
{
	timer_init();
	kbd_init();
	shift_init();
	sensor_init();
	menu_init();
	menu_items_init();

	beep_init();

    reset_settings();

	usart0_init(USART_RS485_SLAVE, g_usart0_baudrate);
	usart1_init(USART_RS485_SLAVE, g_usart1_baudrate);
	
	GLOBAL_INT_ENABLE;

	beep_ms(1000);
	_delay_ms(500);
	beep_ms(200);
	_delay_ms(200);
	beep_ms(200);
	_delay_ms(200);


	lcd_init();
	
	for (;;)
	{
		do_lcd();
        do_sensor();
        do_shift();
        sprintf(lcd_line0, "L0:%d:%d     ", m0, timer_seconds_total);
        sprintf(lcd_line1, "L1:%d:%d     ", m1, timer_seconds_total);
        
        
        for (idx = 0; idx < 4; idx++)
            if (!TEST_SENSOR(idx)) {
                beep_ms(50);
                _delay_ms(100);
                SETBIT(controls, idx);
                CLEARBIT(controls, idx + 4);
                sprintf(lcd_line0, "L0:%d:%d:%d     ", m0, timer_seconds_total, idx);
            } else {
                SETBIT(controls, idx + 4);
                CLEARBIT(controls, idx);

            }

//		menu_doitem();
		process_usart();
	}
	return 0;
}


void do_timer(void)
{
//	do_kbd();
}

void process_usart(void)
{
    char    *pos;
    if (usart0_msg_ready)
    {
        usart0_msg_ready = 0;
        m0++;
        
        usart0_write();
        pos = lcd_line0;
        while (*pos)
            usart0_putchar(*pos++);
        usart0_putchar('\n');
        usart0_read();
    }
    if (usart1_msg_ready)
    {
        usart1_msg_ready = 0;
        m1++;
        usart1_write();
        pos = lcd_line1;
        while (*pos)
            usart1_putchar(*pos++);
        usart1_putchar('\n');
        usart1_read();
    }
}


