#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include "i8254.h"
#include "timer.h"

int timer_hook_id=0;
int counter;

int timer_set_frequency(unsigned char timer, unsigned long freq) {

	if(timer < 0 || timer > 2) {
		printf("Invalid timer\n");
		return 1;
	}
	if(freq < 0) {
		printf("Invalid frequency\n");
		return 1;
	}

	unsigned long new_freq = TIMER_FREQ/freq;
	unsigned char timer_conf;
	if(OK != timer_get_conf(timer, &timer_conf)) {
		printf("timer_get_conf failed\n");
		return 1;
	}
	unsigned long selected_timer;
	unsigned char new_conf;
	if(timer == 0) {
		new_conf = TIMER_SEL0;
		selected_timer = TIMER_0;
	}
	if(timer == 1) {
		new_conf = TIMER_SEL1;
		selected_timer = TIMER_1;
	}
	if(timer == 2) {
		new_conf = TIMER_SEL2;
		selected_timer = TIMER_2;
	}

	new_conf |= TIMER_LSB_MSB;
	new_conf |= (BIT(3) | BIT(2) | BIT(1) | BIT(0)) & timer_conf;

	if(OK != sys_outb(TIMER_CTRL, new_conf)) {
		printf("Error writing new timer config\n");
		return 1;
	}
	if(OK != sys_outb(selected_timer, new_freq & 0xff)) {
		printf("Error writing LSB on selected timer\n");
		return 1;
	}
	if(OK != sys_outb(selected_timer, new_freq >> 8)) {
		printf("Error writing MSB on selected timer\n");
		return 1;
	}

	return 0;
}

int timer_subscribe_int(void ) {
	int timer_hook_id_tmp=timer_hook_id;
	if (OK==sys_irqsetpolicy(TIMER0_IRQ,  IRQ_REENABLE, &timer_hook_id)) {
		if (OK==sys_irqenable(&timer_hook_id))
			return BIT(timer_hook_id_tmp);
	}
	printf("timer_subscribe failed\n");
	return 1;
}

int timer_unsubscribe_int() {
	int timer_hook_id_tmp=timer_hook_id;
	if (OK==sys_irqdisable(&timer_hook_id)) {
		if (OK == sys_irqrmpolicy(&timer_hook_id_tmp))
			return BIT(timer_hook_id_tmp);
	}
	printf("timer_unsubscribe failed\n");
	return 1;
}

void timer_int_handler() {
	counter++;
}

int timer_get_conf(unsigned long timer, unsigned char *st) {

	if (timer<0 || timer>2) {
		printf("Invalid timer\n");
		return 1;
	}
	unsigned long command=TIMER_RB_SEL(timer) | TIMER_RB_CMD | TIMER_RB_COUNT_;
	unsigned long conf;
	
	if (sys_outb(TIMER_CTRL, command)!=OK) {
		printf("Failed to write command to timer controller\n");
		return 1;
	}

	switch(timer) {
	case 0:
		if (sys_inb(TIMER_0, &conf) != OK) {
			printf("Sys in failed!\n");
			return 1;
		}
		break;
	case 1:
		if (sys_inb(TIMER_1, &conf) != OK) {
			printf("Sys in failed!\n");
			return 1;
		}
		break;
	case 2:
		if (sys_inb(TIMER_2, &conf) != OK) {
			printf("Sys in failed!\n");
			return 1;
		}
		break;
	}

	*st=conf;

	return 0;
}

int timer_display_conf(unsigned char conf) {
	unsigned int OPERATING_MODE = (BIT(3) | BIT(2) | BIT(1)) & conf;

	printf("Output value: %d \n", BIT(7) & conf);

	printf("Nullcount value: %d \n", BIT(6) & conf);

	if (conf & TIMER_LSB)
		printf("Type of access: LSB\n");
	else if (conf & TIMER_MSB)
		printf("Type of access: MSB\n");
	else if (conf & TIMER_LSB_MSB)
		printf("Type of access: LSB followed by MSB\n");

	if (OPERATING_MODE == 0)
		printf("Mode 0: Interrupt on terminal count\n");
	else if (OPERATING_MODE == BIT(1))
		printf("Mode 1: Hardware retriggerable one-shot\n");
	else if (OPERATING_MODE == BIT(2))
		printf("Mode 2: Rate Generator\n");
	else if (OPERATING_MODE == (BIT(2) | BIT(1)))
		printf("Mode 3: Square Wave Mode\n");
	else if (OPERATING_MODE == BIT(3))
		printf("Mode 4: Software triggered strobe\n");
	else if(OPERATING_MODE == (BIT(3) | BIT(1)))
		printf("Mode 5: Hardware triggered strobe (retriggerable)\n");

	if((BIT(0) & conf) == 0)
		printf("Counting mode: Binary (16 bits)\n");
	else
		printf("Counting mode: BCD (4 decades)\n");

	return 0;
}

int timer_test_time_base(unsigned long freq) {
	if(OK != timer_set_frequency(0, freq)) {
		printf("Timer_set_frequency failed\n");
		return 1;
	}
	return 0;
}

int timer_test_int(unsigned long time) {
	counter = 0;
	int ipc_status;
	message msg;
	int irq_set = timer_subscribe_int();
	int r;

	if(time <= 0) {
		printf("Invalid time\n");
		return 1;
	}

	while(counter < time*60) {
		/* Get a request message. */
		if( (r = driver_receive(ANY, &msg ,&ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if(is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if(msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
						timer_int_handler();
						/*if((counter % 60) == 0) {
							if(counter / 60 == 1)
								printf("1 second has passed\n");
							else
								printf("%d seconds have passed\n", counter/60);
						}*/
					}
					break;
				default:
					break;  /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}
	timer_unsubscribe_int();
	return 0;
}

int timer_test_config(unsigned long timer) {
	unsigned char conf;

	if(timer < 0 || timer > 2) {
		printf("Invalid timer\n");
		return 1;
	}
	
	if(OK != timer_get_conf(timer, &conf)) {
		printf("timer_get_conf failed");
		return 1;
	}

	timer_display_conf(conf);

	return 0;
}
