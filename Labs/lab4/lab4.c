#include <minix/drivers.h>
#include "test4.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static unsigned short parse_ushort(char *str, int base);
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
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"test_packet <unsigned short cnt>\" \n "
			"\t service run %s -args \"test_async <unsigned short idle_time>\" \n "
			"\t service run %s -args \"test_remote <unsigned long period, unsigned short cnt>\" \n "
			"\t service run %s -args \"test_gesture <short length>\" \n ", argv[0],
			argv[0], argv[0], argv[0]);

}

static int proc_args(int argc, char **argv)
{
	unsigned long period;
	unsigned short cnt, idle;
	short length;
	if (strncmp(argv[1], "test_packet", strlen("test_packet")) == 0) {
		if (argc != 3) {
			printf("mouse: wrong no. of arguments for mouse_test_packet()\n");
			return 1;
		}
		cnt = parse_ushort(argv[2], 10);
		if (cnt == USHRT_MAX)
			return 1;
		printf("mouse::mouse_test_packet(%d)\n", cnt);
		return mouse_test_packet(cnt);
	}
	else if (strncmp(argv[1], "test_async", strlen("test_async")) == 0) {
		if (argc != 3) {
			printf("mouse: wrong no. of arguments for mouse_test_async()\n");
			return 1;
		}
		idle = parse_ushort(argv[2], 10);
		if (idle == USHRT_MAX)
			return 1;
		printf("mouse::mouse_test_async(%d)\n", idle);
		return mouse_test_async(idle);
	}
	else if (strncmp(argv[1], "test_remote", strlen("test_remote")) == 0) {
		if (argc != 4) {
			printf("mouse: wrong no of arguments for mouse_test_remote()\n");
			return 1;
		}
		period = parse_ulong(argv[2], 10);
		if (period == ULONG_MAX)
			return 1;
		cnt = parse_ushort(argv[3], 10);
		if (cnt == USHRT_MAX)
			return 1;
		printf("mouse::mouse_test_remote()\n");
		return mouse_test_remote(period, cnt);
	}
	else if (strncmp(argv[1], "test_gesture", strlen("test_gesture")) == 0) {
		if (argc != 3) {
			printf("Mouse: wrong no of arguments for test_gesture(short length)\n");
			return 1;
		}
		length = parse_ulong(argv[2], 10);
		if (length == ULONG_MAX)
			return 1;
		printf("Mouse::test_gesture(%lu)\n", length);
		return mouse_test_gesture(length);
	}
	else {
		printf("timer: %s - no valid function!\n", argv[1]);
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
		printf("mouse: parse_ulong: no digits were found in %s\n", str);
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

	if ((errno == ERANGE && val == USHRT_MAX ) || (errno != 0 && val == 0)) {
		perror("strtous");
		return USHRT_MAX;
	}

	if (endptr == str) {
		printf("mouse: parse_ushort: no digits were found in %s\n", str);
		return USHRT_MAX;
	}

	/* Successful conversion */
	return val;
}

//TODO parse_short
