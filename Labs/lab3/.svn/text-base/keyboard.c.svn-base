#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include "i8042.h"
#include "keyboard.h"

int keyboard_hook_id=1;
int cnt = 0;

int sys_inb_cnt(port_t port, unsigned long *byte) {
	cnt++;
	return sys_inb(port,byte);
}

int keyboard_subscribe_int(void ) {
	int keyboard_hook_id_tmp=keyboard_hook_id;
	if (OK==sys_irqsetpolicy(KBD_IRQ,  IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id)) {
		if (OK==sys_irqenable(&keyboard_hook_id))
			return BIT(keyboard_hook_id_tmp);
	}
	printf("keyboard_subscribe failed\n");
	return 1;
}

int keyboard_unsubscribe_int() {
	int keyboard_hook_id_tmp=keyboard_hook_id;
	if (OK==sys_irqdisable(&keyboard_hook_id)) {
		if (OK == sys_irqrmpolicy(&keyboard_hook_id_tmp))
			return BIT(keyboard_hook_id_tmp);
	}
	printf("keyboard_unsubscribe failed\n");
	return 1;
}

unsigned long keyboard_read() {
	unsigned long stat;
	unsigned long key;
	int i = NTRIES;
	while (i) {
		if (sys_inb_cnt(STAT_REG, &stat) != OK) {
			printf("Sys in failed!\n");
			return 1;
		}
		if (stat & OBF) {
			if (sys_inb_cnt(OUT_BUF, &key) != OK) {
				printf("Sys in failed!\n");
				return 1;
			}

			if ((stat & (TIMEOUT | PARITY)) == 0) {

				return key;
			}
		}
		tickdelay(micros_to_ticks(DELAY_US));
		i--;

	}
	return 1;
}


int printScancodes(unsigned long key, unsigned long aux) {

	if (key & BIT(7)) {
		if (aux == TWOBYTE) {
			printf("Breakcode: 0x%x 0x%x\n", aux, key);
		} else if (key != TWOBYTE) {
			printf("Breakcode: 0x%x\n", key);
		}
		aux = key;
	} else {
		if (aux == TWOBYTE) {
			printf("Makecode: 0x%x 0x%x\n", aux, key);
		} else if (key != TWOBYTE) {
			printf("Makecode: 0x%x\n", key);
		}
		aux = key;
	}

	return aux;
}


int writeCMDtoKBD(unsigned long port, unsigned long cmd) {
	unsigned long stat;
	int i = 10;

	while( i ) {
		if(sys_inb(STAT_REG, &stat) != OK) {
			return 1;
		}
		/* loop while 8042 input buffer is not empty */
		if( (stat & IBF) == 0 ) {
			if(sys_outb(port, cmd) == OK)
				return 0;
		}
		i--;
		tickdelay(micros_to_ticks(DELAY_US));
	}
	return 1;
}
