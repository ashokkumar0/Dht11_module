#include "timer32.h"
#include "dht11.h"
#include <stdbool.h>

bool state;

void dht11_init(void)
{
	bool state = 0;
	timer32_init();
	timer32_reset();
}

int dht11_read(uint8_t *data)
{	
	int i = 0;
	int j = 0;
	int val = 0;
	volatile uint32_t * port2_dir  = (uint32_t *) GPIO2DIR;
	volatile uint32_t * port2_data = (uint32_t *) GPIO2DATA;
	for (i = 0; i < 5; i++)
		data[i] = 0;
	timer32_reset();
	if (state == 0) {
		state = 1;
		while (timer32_mread() < 1500)
			;
	}
	*port2_dir |= 0x1;
	*port2_data &= ~(0x1);
	timer32_reset();
	while (timer32_mread() < 20)
		;
	*port2_dir &= ~(0x1);
	timer32_reset();
	while(*port2_data & 0x1)
		if (timer32_uread() > 10000)
			return -1;
	timer32_reset();
	while(!(*port2_data & 0x1))
		if(timer32_uread() > 10000)
			return -1;
	timer32_reset();
	while(*port2_data & 0x1)
		if (timer32_uread() > 10000)
			return -1;
	timer32_reset();
	while(!(*port2_data & 0x1))
		if(timer32_uread() > 10000)
			return -1;
	timer32_reset();
	while(*port2_data & 0x1)
		if (timer32_uread() > 10000)
			return -1;		
	for (i = 0; i < 5; i++) {
		for (j = 7; j >= 0; j--) {
			timer32_reset();	
			while(!(*port2_data & 0x1)) 
				if(timer32_uread() > 10000)
					return -1;
			timer32_reset();
			while(*port2_data & 0x1) 		
				if (timer32_uread() > 60) 
					data[i] |= (1 << j);				
		}
	}
	val = data[0] + data[2];
	if (!(val == data[4]))
		return -1;
	return 0;
}

uint8_t dht11_get_humidity(void)
{
	uint8_t data[5];
	int i;
	for (i = 0; i < 5; i++) 
		if(!(dht11_read(data)))
			return data[0];
	return -1;		   
}

uint8_t dht11_get_temperature(void)
{
	uint8_t data[5];
	int i;
	for (i = 0; i < 5; i++) 
		if(!(dht11_read(data)))
			return data[2];
	return -1;
}
