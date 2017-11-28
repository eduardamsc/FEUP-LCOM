#include "video_gr.h"
#include <unistd.h>
#include <stdio.h>
#include "timer.h"
#include "keyboard.h"
#include "test5.h"
#include "read_xpm.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include "i8042.h"
#include "vbe.h"
#include "video_test.h"

int video_test_init(unsigned short mode, unsigned short delay) {
	if (mode < MIN_MODE || mode > MAX_MODE) {
		printf("mode must be between 0x%x and 0x%x.\n", MIN_MODE, MAX_MODE);
		return 1;
	}
	vg_init(mode);
	timer_test_int(delay);
	vg_exit();
	return 1;
}

int video_test_square(unsigned short x, unsigned short y, unsigned short size,
		unsigned long color) {
	if (color > MAX_COLOR) {
		printf("color must be between inferior to 0x40.\n");
		return 1;
	}

	vg_init(DEFAULTMODE);


	unsigned int i, j;
	for (i = x; i < size + x; i++) {
		for (j = y; j < size + y; j++) {
			if (check_coordinates(i + HRES / 2 - size / 2,
					j + VRES / 2 - size / 2) == 0)
				paint_pixel(i + HRES / 2 - size / 2, j + VRES / 2 - size / 2,
						color);
		}
	}
	video_dump_fb();
	kbd_test_scan(0);
	vg_exit();
	return 0;

}

int video_test_line(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, unsigned long color) {

	if (color < 0 || color > MAX_COLOR) {
		printf("color must be between 0 and 0x3f.\n");
		return 1;
	}

	vg_init(DEFAULTMODE);

	int d; /* Decision variable */
	int dx, dy; /* Dx and Dy values for the line */
	int Eincr, NEincr; /* Decision variable increments */
	int yincr; /* Increment for y values */
	int t; /* Counters etc. */

	dx = ABS2(xf - xi);
	dy = ABS2(yf - yi);
	if (dy <= dx) {
		/* We have a line with a slope between -1 and 1
		 *
		 * Ensure that we are always scan converting the line from left to
		 * right to ensure that we produce the same line from P1 to P0 as the
		 * line from P0 to P1.
		 */
		if (xf < xi) {
			t = xf;
			xf = xi;
			xi = t; /* Swap X coordinates */
			t = yf;
			yi = yi;
			yi = t; /* Swap Y coordinates */
		}
		if (yf > yi)
			yincr = 1;
		else
			yincr = -1;
		d = 2 * dy - dx; /* Initial decision variable value */
		Eincr = 2 * dy; /* Increment to move to E pixel */
		NEincr = 2 * (dy - dx); /* Increment to move to NE pixel */
		if (check_coordinates(xi, yi) == 0)
			paint_pixel(xi, yi, color); /* Draw the first point at (x1,y1) */

		/* Incrementally determine the positions of the remaining pixels */
		for (xi++; xi <= xf; xi++) {
			if (d < 0)
				d += Eincr; /* Choose the Eastern Pixel */
			else {
				d += NEincr; /* Choose the North Eastern Pixel */
				yi += yincr; /* (or SE pixel for dx/dy < 0!) */
			}
			if (check_coordinates(xi, yi) == 0)
				paint_pixel(xi, yi, color); /* Draw the point */
		}
		video_dump_fb();
	} else {
		/* We have a line with a slope between -1 and 1 (ie: includes
		 * vertical lines). We must swap our x and y coordinates for this.
		 *
		 * Ensure that we are always scan converting the line from left to
		 * right to ensure that we produce the same line from P1 to P0 as the
		 * line from P0 to P1.
		 */
		if (yf < yi) {
			t = xf;
			xf = xi;
			xi = t; /* Swap X coordinates */
			t = yf;
			yf = yi;
			yi = t; /* Swap Y coordinates */
		}
		if (xf > xi)
			yincr = 1;
		else
			yincr = -1;
		d = 2 * dx - dy; /* Initial decision variable value */
		Eincr = 2 * dx; /* Increment to move to E pixel */
		NEincr = 2 * (dx - dy); /* Increment to move to NE pixel */
		if (check_coordinates(xi, yi) == 0)
			paint_pixel(xi, yi, color); /* Draw the first point at (x1,y1) */
		/* Incrementally determine the positions of the remaining pixels */
		for (yi++; yi <= yf; yi++) {
			if (d < 0)
				d += Eincr; /* Choose the Eastern Pixel */
			else {
				d += NEincr; /* Choose the North Eastern Pixel */
				xi += yincr; /* (or SE pixel for dx/dy < 0!) */
			}
			if (check_coordinates(xi, yi) == 0)
				paint_pixel(xi, yi, color); /* Draw the point */
		}
		video_dump_fb();
	}

	kbd_test_scan(0);
	vg_exit();
	return 0;

}

int test_xpm(char *xpm[], unsigned short xi, unsigned short yi) {
	int width, height;

	vg_init(DEFAULTMODE);

	char * pixmap = read_xpm(xpm, &width, &height);

	unsigned int i, j;
	for (i = 0; i < width; i++) {
		for (j = 0; j < height; j++) {
			if (check_coordinates(i + xi, j + yi) == 0)
				paint_pixel(i + xi, j + yi, *(pixmap + i + j * width));
		}
	}
	video_dump_fb();
	kbd_test_scan(0);
	vg_exit();
	return 0;

}

int test_move(char *xpm[], unsigned short xi, unsigned short yi,
		unsigned short xf, unsigned short yf, short speed,
		unsigned short frame_rate) {

	int width, height;
	int i, j;

	timer_set_frequency(0, frame_rate);
	unsigned short increment = speed;
	int counter = 0;
	unsigned short frames_needed;

	if (speed < 0) {
		frames_needed = -speed;
		increment = 1;
	}
	char * pixmap = read_xpm(xpm, &width, &height);

	int ipc_status;
	message msg;
	int r;
	unsigned long key;
	int kbd_irq_set = keyboard_subscribe_int();
	int timer_irq_set = timer_subscribe_int();
	vg_init(DEFAULTMODE);
	while (key != ESC) {
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
				}
				if (msg.NOTIFY_ARG & timer_irq_set) {
					if (speed > 0 || counter == frames_needed) {
						if (xi < xf) {
							/////CLEAN OLD IMAGE
							for (i = 0; i < width; i++) {
								for (j = 0; j < height; j++) {
									if (check_coordinates(i + xi - increment,
											j + yi) == 0)
										paint_pixel(i + xi - increment, j + yi,
												0x00);
								}
							}
							/////PAINT NEW IMAGE
							for (i = 0; i < width; i++) {
								for (j = 0; j < height; j++) {
									if (check_coordinates(i + xi, j + yi) == 0)
										paint_pixel(i + xi, j + yi,
												*(pixmap + i + j * width));
								}
							}
							video_dump_fb();
							xi += increment;
						}
						if (yi < yf) {
							//////CLEAN OLD IMAGE
							for (i = 0; i < width; i++) {
								for (j = 0; j < height; j++) {
									if (check_coordinates(i + xi,
											j + yi - increment) == 0)
										paint_pixel(i + xi, j + yi - increment,
												0x00);
								}
							}
							/////PAINT NEW IMAGE
							for (i = 0; i < width; i++) {
								for (j = 0; j < height; j++) {
									if (check_coordinates(i + xi, j + yi) == 0)
										paint_pixel(i + xi, j + yi,
												*(pixmap + i + j * width));
								}
							}
							video_dump_fb();
							yi += increment;
						}
						counter = 0;
					} else {
						counter++;
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
	vg_exit();
	timer_set_frequency(0, 60);
	timer_unsubscribe_int();
	keyboard_unsubscribe_int();

	return 0;
}

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)
int test_controller() {
	vbe_info_block* vbe_info = (vbe_info_block*) malloc(sizeof(vbe_info_block));
	vbe_get_controller_info(vbe_info);

	printf("%x.%x\n", (((*vbe_info).VbeVersion & 0xFF00) >> 8),
			((*vbe_info).VbeVersion & 0x00FF));

	int i = 0;
	while (((*vbe_info).reserved[i] != -1)) {
		printf("0x%x:", (*vbe_info).reserved[i]);
		i++;
	}
	printf("\n%d\n", (*vbe_info).TotalMemory * 64); //Number of 64kb memory blocks
	return 0;

}
