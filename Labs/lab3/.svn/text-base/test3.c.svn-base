#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include "i8254.h"
#include "i8042.h"
#include "test3.h"
#include "keyboard.h"
#include "timer.h"

extern int cnt;

int kbd_test_scan(unsigned short assembly) {

	int ipc_status;
	message msg;
	int irq_set = keyboard_subscribe_int();
	int r;
	unsigned long aux = 0;

	while (key_asm != ESC) {
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
					if (!assembly) {  //C
						key_asm = keyboard_read();
						aux = printScancodes(key_asm, aux);

					} else if(assembly){ //assembly
						keyboard_read_ass();
						aux = printScancodes(key_asm, aux);
					}
					else {
						printf("Arguments are 0 or 1, for C or assembly, respectively\n");
						return 1;
					}
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}
	if (!assembly)
		printf("Number of sys_in calls: %d\n", cnt);
	keyboard_unsubscribe_int();

	return 0;
}

int kbd_test_poll() {
	unsigned long key;
	unsigned long cmd_byte;
	unsigned long aux = 0;

	while (key != ESC) {
		key = keyboard_read();
		if (key != 1)
			aux = printScancodes(key, aux);

		tickdelay(micros_to_ticks(DELAY_US));
	}
	printf("Number of sys_in calls: %d\n", cnt);

	writeCMDtoKBD(KBC_CMD_REG, READ_CMD_BYTE);

	if (sys_inb(OUT_BUF, &cmd_byte) != OK) {
		return 1;
	}

	cmd_byte |= BIT(0);
	if (writeCMDtoKBD(KBC_CMD_REG, WRITE_CMD_BYTE) != OK)
		return 1;
	if (writeCMDtoKBD(IN_BUF, cmd_byte) != OK)
		return 1;

	return 0;
}

int kbd_test_timed_scan(unsigned short n) {
	int ipc_status;
	message msg;
	int kbd_irq_set = keyboard_subscribe_int();
	int timer_irq_set = timer_subscribe_int();
	int counter = n * 60;
	int r;
	unsigned long key;
	unsigned long aux = 0;

	while (key != ESC && counter != 0) { //C
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & kbd_irq_set) { /* subscribed interrupt */

					key = keyboard_read();
					aux = printScancodes(key, aux);

					counter = n * 60;

				}

				if (msg.NOTIFY_ARG & timer_irq_set) {
					counter--;
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}
	//printf("Number of sys_in calls: %d\n", cnt);
	if (counter == 0) {
		printf("\nTime Out ...\n");
	}
	keyboard_unsubscribe_int();
	timer_unsubscribe_int();
	return 1;
}
