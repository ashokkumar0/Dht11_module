#include <board.h>
#include <lcd.h>
#include "dht11.h"

int main(void)
{
	board_init();
	lcd_init();
	dht11_init();
	board_stdout(LCD_STDOUT);
	while (1) {
		printf("%u\n", dht11_get_temperature());
	}
	return 0;
}
