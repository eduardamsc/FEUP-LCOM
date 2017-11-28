unsigned long packet[3];

int mouse_subscribe_int();

int mouse_unsubscribe_int();

int print_packets();

int mouse_write(unsigned long cmd);

int mouse_read(unsigned long* data);

int mouse_handler();

int calculateLength(short *curr_Length);

//int writeCMDtoKBC(unsigned long port, unsigned long cmd);
