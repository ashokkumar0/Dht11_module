#include <board.h>
#include <lcd.h>
#include <delay.h>
#include "timer32.h"

volatile uint32_t * port2_dir  = (uint32_t *) 0x50028000;
volatile uint32_t * port2_data = (uint32_t *) 0x50023ffc;
volatile uint32_t * port2_config = (uint32_t *) 0x40044008;
bool a[41];

void dht11_init()
{
	timer32_init();
	timer32_reset();
}

int dht11_read(uint8_t *data)
{	
	int val = 0;
	int i = 0;
	int j = 0;
	
	for (i = 0; i < 5; i++)
		data[i] = 0;
	timer32_reset();
	while (timer32_mread() < 1500)
		;
	*port2_dir |= 0x1;
	*port2_data &= ~(0x1);
	//mdelay(20);
	timer32_reset();
	while (timer32_mread() < 20)
		;
	//*port2_data |= 0x1;
	timer32_reset(); 
	while (timer32_mread() < 2)
		;
	 *port2_dir &= ~(0x1);
	 //mdelay(9);
	timer32_reset();
	while(*port2_data & 0x1)
		if (timer32_uread() > 10000)
			return 15;
	timer32_reset();
	while(!(*port2_data & 0x1))
		if(timer32_uread() > 10000)
			return 99;
	timer32_reset();
	while(*port2_data & 0x1)
		if (timer32_uread() > 10000)
			return 100;
	timer32_reset();
	while(!(*port2_data & 0x1))
		if(timer32_uread() > 10000)
			return 99;
	timer32_reset();
	while(*port2_data & 0x1)
		if (timer32_uread() > 10000)
			return 100;		
	for (i = 0; i < 5; i++) {
		for (j = 7; j >= 0; j--) {
			timer32_reset();	
			while(!(*port2_data & 0x1)) 
				if(timer32_uread() > 10000)
					return 2;
			timer32_reset();
			while(*port2_data & 0x1) 		
				if (timer32_uread() > 60) {
					data[i] |= (1 << j);
				}
		}
	}
	for (i = 0; i < 5; i++)
		printf("%u ",(char) (data[i]));
	//printf("\n%d %d\n",(data[0]+data[2]),data[4]);
	return 4;
}

int main(void)
{
	uint8_t data[5];
	
	board_init();
	dht11_init();
	lcd_init();
	delay_init();
	board_stdout(LCD_STDOUT);
	//*port2_config &= ~(0x18);
	//*port2_config |= (1 << 10);
	while (1)
		printf("%d\n", dht11_read(data));
	return 0;
}
