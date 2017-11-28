#include <minix/drivers.h>
#include "test5.h"
#include "pixmap.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static unsigned short parse_ushort(char *str, int base);
static signed long parse_long(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char** argv) {
	sef_startup();

	if (argc == 1) {
		print_usage(argv);
		return 0;
	} else {
		proc_args(argc, argv);
	}
	return 0;
}

static void print_usage(char *argv[]) {
	printf(
			"Usage: one of the following:\n"
			"\t service run %s -args \"test_init <unsigned short mode, unsigned short delay>\" \n "
			"\t service run %s -args \"test_square <unsigned short x, unsigned short y, unsigned short size, unsigned long color>\" \n "
			"\t service run %s -args \"test_line <unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color>\" \n "
			"\t service run %s -args \"test_xpm <char *xpm[], unsigned short xi, unsigned short yi>\"\n"
			"\t service run %s -args \"test_move <char *xpm[], unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, short speed, unsigned short frame_rate>\"\n"
			"\t service run %s -args \"test_controller\"\n",
			argv[0], argv[0], argv[0], argv[0], argv[0], argv[0]);

}

static int proc_args(int argc, char **argv) {
	unsigned short mode, delay;
	unsigned short x, y, size;
	unsigned long color;
	unsigned short xi, yi, xf, yf;

	if (strncmp(argv[1], "test_init", strlen("test_init")) == 0) {
		if (argc != 4) {
			printf("video: wrong no. of arguments for video_test_init()\n");
			return 1;
		}
		mode = parse_ushort(argv[2], 16);
		delay = parse_ushort(argv[3], 10);
		if (mode == USHRT_MAX || delay == USHRT_MAX)
			return 1;
		printf("video::video_test_init(%d,%d)\n", mode, delay);
		return video_test_init(mode, delay);
	} else if (strncmp(argv[1], "test_square", strlen("test_square")) == 0) {
		if (argc != 6) {
			printf("video: wrong no. of arguments for video_test_square()\n");
			return 1;
		}
		x = parse_ushort(argv[2], 10);
		y = parse_ushort(argv[3], 10);
		size = parse_ushort(argv[4], 10);
		if (x == USHRT_MAX || y == USHRT_MAX || size == USHRT_MAX)
			return 1;

		color = parse_ulong(argv[5], 16);
		if (color == ULONG_MAX)
			return 1;

		printf("video::video_test_square(%d,%d,%d,%d)\n", x, y, size, color);
		return video_test_square(x, y, size, color);
	} else if (strncmp(argv[1], "test_line", strlen("test_line")) == 0) {
		if (argc != 7) {
			printf("video: wrong no of arguments for video_test_line()\n");
			return 1;
		}
		xi = parse_ushort(argv[2], 10);
		yi = parse_ushort(argv[3], 10);
		xf = parse_ushort(argv[4], 10);
		yf = parse_ushort(argv[5], 10);
		if (xi == USHRT_MAX || yi == USHRT_MAX || xf == USHRT_MAX
				|| yf == USHRT_MAX)
			return 1;

		color = parse_ulong(argv[6], 16);
		if (color == ULONG_MAX)
			return 1;

		printf("video::video_test_line(%d,%d,%d,%d,%d)\n", xi, yi, xf, yf, color);
		return video_test_line(xi, yi, xf, yf, color);
	}
	else if (strncmp(argv[1], "test_xpm", strlen("test_xpm")) == 0) {
		if (argc != 5) {
			printf("lab5: wrong no of arguments for test_xpm()\n");
			return 1;
		}

		char ** pic;

		if ( strncmp(argv[2], "pic1", strlen("pic1")) == 0 ) {
			pic = pic1;
		} else if ( strncmp(argv[2], "pic2", strlen("pic2")) == 0 ) {
			pic = pic2;
		} else if ( strncmp(argv[2], "cross", strlen("cross")) == 0 ) {
			pic = cross;
		} else if ( strncmp(argv[2], "pic3", strlen("pic3")) == 0 ) {
			pic = pic3;
		} else if ( strncmp(argv[2], "penguin", strlen("penguin")) == 0 ) {
			pic = penguin;
		} else if ( strncmp(argv[2], "car", strlen("car")) == 0 ) {
			pic = car;
		} else {
			printf("lab5: incorrect image.\n");
			return 1;
		}

		xi = parse_ushort(argv[3], 10);
		yi = parse_ushort(argv[4], 10);

		if (xi == USHRT_MAX || yi == USHRT_MAX)
			return 1;


		printf("lab5::test_xpm(%s, %d, %d)\n", argv[2], xi , yi);
		return test_xpm(pic, xi, yi);
	}

	else if (strncmp(argv[1], "test_move", strlen("test_move")) == 0) {

		//test_move <char *xpm[], unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, short s, unsigned short f>\"\n"
		if (argc != 9) {
			printf("lab5: wrong no of arguments for test_move()\n");
			return 1;
		}

		char ** pic;

		if ( strncmp(argv[2], "pic1", strlen("pic1")) == 0 ) {
			pic = pic1;
		} else if ( strncmp(argv[2], "pic2", strlen("pic2")) == 0 ) {
			pic = pic2;
		} else if ( strncmp(argv[2], "cross", strlen("cross")) == 0 ) {
			pic = cross;
		} else if ( strncmp(argv[2], "pic3", strlen("pic3")) == 0 ) {
			pic = pic3;
		} else if ( strncmp(argv[2], "penguin", strlen("penguin")) == 0 ) {
			pic = penguin;
		} else {
			printf("lab5: incorrect image.\n");
			return 1;
		}

		long speed;
		unsigned short frame_rate;

		xi = parse_ushort(argv[3], 10);
		yi = parse_ushort(argv[4], 10);
		xf = parse_ushort(argv[5], 10);
		yf = parse_ushort(argv[6], 10);
		speed = parse_long(argv[7], 10);
		frame_rate = parse_ushort(argv[8], 10);

		if (xi == USHRT_MAX || yi == USHRT_MAX || xf == USHRT_MAX || yf == USHRT_MAX || speed == SHRT_MAX || speed == SHRT_MIN || frame_rate == USHRT_MAX)
			return 1;

		printf("lab5::test_move(%s, %d, %d, %d, %d, %d, %d)\n", argv[2], xi, xf, yi, yf, speed, frame_rate);
		return test_move(pic, xi, yi, xf, yf, speed, frame_rate);
	} else if (strncmp(argv[1], "test_controller", strlen("test_controller")) == 0) {
			if (argc != 2) {
				printf("lab5: wrong no of arguments for test_controller()\n");
				return 1;
			}
			printf("lab5::test_controller()\n");
			return test_controller();
		}


	else {
		printf("video: %s - no valid function!\n", argv[1]);
		return 1;
	}
}

static unsigned long parse_ulong(char *str, int base) {
	char *endptr;
	unsigned long val;

	/* Convert string to unsigned long */
	val = strtoul(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return ULONG_MAX;
	}

	if (endptr == str) {
		printf("video: parse_ulong: no digits were found in %s\n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}

static unsigned short parse_ushort(char *str, int base) {
	char *endptr;
	unsigned long val;

	/* Convert string to unsigned short */
	val = strtoul(str, &endptr, base);

	if ((errno == ERANGE && val == USHRT_MAX) || (errno != 0 && val == 0)) {
		perror("strtous");
		return USHRT_MAX;
	}

	if (endptr == str) {
		printf("video: parse_ushort: no digits were found in %s\n", str);
		return USHRT_MAX;
	}

	/* Successful conversion */
	return val;
}

static signed long parse_long(char *str, int base) {
	char *endptr;
	unsigned long val;

	/*Convert string to signed long*/
	val = strtol(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == LONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtol");
		return LONG_MAX;
	}
	if ((errno == ERANGE && val == LONG_MIN) || (errno != 0 && val == 0)) {
		perror("strtol");
		return LONG_MIN;
	}

	if (endptr == str) {
		printf("lab5: parse_long: no digits were found in %s\n", str);
		return LONG_MAX;	//Could also be LONG_MIN
	}

	/* Successful conversion */
	return val;
}
