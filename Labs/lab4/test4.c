#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include "i8042.h"
#include "test4.h"
#include "mouse.h"
#include "timer.h"
#include <minix/com.h>
#include "kbc.h"

typedef enum {INIT, DRAW, COMP} state_t;
typedef enum {RDOWN, RUP, MOVE} ev_type_t;

struct event_t {
	ev_type_t type;
	short length;
	short curr_length;
	short previous_X;
	short previous_Y;
};

static state_t st = INIT;
void check_length(struct event_t* evt) {
	switch (st) {
	case INIT:
		if (evt->type == RDOWN)
			st = DRAW;
		break;

	case DRAW:
		if (evt->type == RDOWN) {
			if(evt->curr_length >= evt->length)
				st = COMP;
		} else if (evt->type == RUP) {
			st = INIT;
			evt->previous_X = 0;
			evt->previous_Y = 0;
		}
		break;
	default:
		break;
	}
}

int mouse_test_packet(unsigned short cnt) {
	if(cnt <= 0)
		printf("Please insert a positive cnt.\n");
	int ipc_status, r;
	message msg;
	int irq_set = mouse_subscribe_int();
	int i = 0;
	int index = 0;

	mouse_write(ENABLE_STREAM);
	mouse_write(ENABLE_DATA_REPORT);

	while (i <= cnt * 3) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					mouse_handler(&index);
					i++;
				}
				break;
			default:
				break;
			}
		} else {

		}
	}
	mouse_unsubscribe_int();
	unsigned long lixo;
	mouse_read(&lixo); //cleaning possible junk
	return 0;
}

int mouse_test_async(unsigned short idle_time) {
	if(idle_time <= 0)
		printf("Please insert a positive idle_time.\n");
	int ipc_status, r;
	message msg;
	int mouse_irq_set = mouse_subscribe_int();
	int timer_irq_set = timer_subscribe_int();

	int index = 0;

	mouse_write(ENABLE_STREAM);
	mouse_write(ENABLE_DATA_REPORT);
	unsigned short time = 0;

	while (time < idle_time * 60) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & mouse_irq_set) {
					mouse_handler(&index);
					time = 0;
				}
				if (msg.NOTIFY_ARG & timer_irq_set) {
					time++;

				}
				break;
			default:
				break;
			}
		} else {
		}
	}
	mouse_unsubscribe_int();
	timer_unsubscribe_int();
	mouse_write(DISABLE_DATA_REPORT);
	unsigned long lixo;
	mouse_read(&lixo); //cleaning possible junk
	return 0;
}

int mouse_test_remote(unsigned long period, unsigned short cnt) {
	if(period <= 0) {
		printf("Please insert a positive period.\n");
	}
	if(cnt <= 0) {
		printf("Please insert a positive cnt.\n");
	}
	unsigned long cmd_byte;
	int i = 0;
	unsigned long data = 0;

	sleep(1);

	mouse_write(ENABLE_DATA_REPORT);
	kbc_read(&cmd_byte);

	unsigned long temp_cmd_byte = cmd_byte;
	temp_cmd_byte &= NOT_BIT1;
	kbc_write(temp_cmd_byte);

	mouse_write(DISABLE_DATA_REPORT);
	mouse_write(ENABLE_REMOTE);
	mouse_read(&data);

	while (i < cnt) {
		mouse_write(READ_DATA);
		mouse_read(&data);
		packet[0] = data;
		mouse_read(&data);
		packet[1] = data;
		mouse_read(&data);
		packet[2] = data;
		i++;
		print_packets();
		tickdelay(micros_to_ticks(period*1000));
	}

	cmd_byte |= BIT(1);
	kbc_write(cmd_byte);
	mouse_write(ENABLE_STREAM);
	return 0;
}

int mouse_test_gesture(short length) {
	if(length <= 0) {
		printf("Function currently supports positive numbers only.\n");
		return 0;
	}

	int ipc_status, r;
	message msg;

	//STATE MACHINE VERSION
	struct event_t mouse_event;
	mouse_event.previous_X = 0;
	mouse_event.previous_Y = 0;
	mouse_event.length = length;
	short pos_currX;
	short pos_currY;

	int irq_set = mouse_subscribe_int();

	int index = 0;

	mouse_write(ENABLE_STREAM);
	mouse_write(ENABLE_DATA_REPORT);

	while(st != COMP) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					if(mouse_handler(&index)==2) {
						if(packet[0] & RIGHTB) {
							pos_currX = (packet[0] & XSIGN)>>4;
							pos_currY = (packet[0] & YSIGN)>>5;

							//either both X and Y positive or both negative
							//if going backwards also resets length
							if((pos_currX != pos_currY)
									|| (pos_currX != mouse_event.previous_X)
									|| (pos_currY != mouse_event.previous_Y)) {
								mouse_event.curr_length = 0;
							}
							else {
								mouse_event.curr_length += packet[1];
							}

							mouse_event.previous_X = pos_currX;
							mouse_event.previous_Y = pos_currY;


							mouse_event.type = RDOWN;
						}
						else {
							mouse_event.type = RUP;
						}

						check_length(&mouse_event);
					}
				}
				break;
			default:
				break;
			}
		} else {

		}
	}
	mouse_unsubscribe_int();
	unsigned long lixo;
	mouse_read(&lixo); //cleaning possible junk
	return 0;
}
