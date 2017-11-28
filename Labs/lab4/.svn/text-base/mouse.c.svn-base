#include "mouse.h"
#include "i8042.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
int mouse_hook_id = 2;

int mouse_subscribe_int() {
	int mouse_hook_id_tmp = mouse_hook_id;
	if (OK
			== sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE,
					&mouse_hook_id)) {
		if (OK == sys_irqenable(&mouse_hook_id))
			return BIT(mouse_hook_id_tmp);
	}
	return 1;
}

int mouse_unsubscribe_int() {
	int mouse_hook_id_tmp = mouse_hook_id;
	if (OK == sys_irqdisable(&mouse_hook_id)) {
		if (OK == sys_irqrmpolicy(&mouse_hook_id_tmp))
			return BIT(mouse_hook_id_tmp);
	}
	return 1;
}

int print_packets() {
	printf("\tB1=0x%x B2=0x%x B3=0x%x ", packet[0], packet[1], packet[2]);
	printf("LB=%d ", (packet[0] & LEFTB) ? 1 : 0);
	printf("MB=%d ", (packet[0] & MIDB) ? 1 : 0);
	printf("RB=%d ", (packet[0] & RIGHTB) ? 1 : 0);
	printf("OVX=%d ", (packet[0] & XOVFL) ? 1 : 0);
	printf("YVX=%d ", (packet[0] & YOVFL) ? 1 : 0);

	if (packet[0] & XSIGN) {
		packet[1] ^= 0xFF;
		packet[1] += 1;
		printf("X=-%d ", packet[1]);
	} else
		printf("X=%d ", packet[1]);

	if (packet[0] & YSIGN) {
		packet[2] ^= 0xFF;
		packet[2] += 1;
		printf("Y=-%d \n", packet[2]);
	} else
		printf("Y=%d \n", packet[2]);

	return 0;
}

int mouse_write(unsigned long cmd) {
	unsigned long stat;
	int i = NTRIES;
	while (i) {
		if (sys_outb(KBC_CMD_REG, WRITE_BYTE_MOUSE) != OK)
			return 1;
		if (sys_inb(OUT_BUF, &stat) != OK)
			return 1;
		if (sys_outb(IN_BUF, cmd) != OK)
			return 1;
		if (sys_inb(OUT_BUF, &stat) != OK)
			return 1;
		if (stat == ACK) {
			return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		i++;
	}
	return 1;
}

int mouse_read(unsigned long* data) {
	unsigned long stat = 0;
	int i = NTRIES;

	while (i) {
		if (sys_inb(STAT_REG, &stat) != OK)
			return 1;
		if ((stat & OBF) && (stat & AUX)) {
			if (sys_inb(OUT_BUF, data) != OK) {
				return 1;
			}
			if ((stat & (TIMEOUT | PARITY)) == OK)
				return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		i--;
	}
	return 1;
}

int mouse_handler(int *index) {
	unsigned long data = 0;
	int temp = *index;
	mouse_read(&data);

	if (temp == 0) {
		if (data & BIT(3)) {
			packet[temp] = data;
			temp++;
		}
	} else if (temp == 2) {
		packet[temp] = data;
		temp = 0;
		print_packets();
		*index = temp;
		return 2;
	} else {
		packet[temp] = data;
		temp++;
	}
	*index = temp;
	return 0;
}
/** HERE LIES A DEAD FUNCTION --> KILLED BY A STATE MACHINE
int calculateLength(short *curr_Length) {
	if (((packet[0] & RIGHTB) == 0)
			|| (((packet[0] & XSIGN)>>4) != ((packet[0] & YSIGN)>>5))) {
		*curr_Length = 0;
		return 1;

	}
	*curr_Length += packet[1]; //packet1 was already changed to correct value in print function

	return 0;
}*/
