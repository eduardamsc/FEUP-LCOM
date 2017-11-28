#include "kbc.h"
int kbc_write(unsigned long cmd) {
	unsigned long stat;
	int i = NTRIES;
	while (i) {
		if (sys_inb(STAT_REG, &stat) != OK)
			return 1;
		if ((stat & IBF) == 0) {
			if (sys_outb(KBC_CMD_REG, WRITE_CMD_BYTE) == OK)
				//return 0;
				break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		i--;
	}
	i = NTRIES;
	while (i) {
		if (sys_inb(STAT_REG, &stat) != OK)
			return 1;
		if ((stat & IBF) == 0) {
			if (sys_outb(IN_BUF, cmd) == OK)
				return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		i--;
	}
	return 1;
}
int kbc_read(unsigned long *data) {
	unsigned long stat;
	int i= NTRIES;

	while (i) {
		if (sys_inb(STAT_REG, &stat) != OK)
			return 1;
		if ((stat & IBF) == 0) {
			if (sys_outb(KBC_CMD_REG, READ_CMD_BYTE) == OK)
			break;
		}
		i--;
		tickdelay(micros_to_ticks(DELAY_US));
	}

	if (sys_inb(OUT_BUF, data) != OK) {
		return 1;
	}

	return 0;
}
