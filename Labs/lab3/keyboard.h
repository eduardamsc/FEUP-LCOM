int keyboard_subscribe_int(void );

int keyboard_unsubscribe_int();

unsigned long keyboard_read();

int printScancodes(unsigned long key, unsigned long aux);

int writeCMDtoKBD(unsigned long port, unsigned long cmd);

extern unsigned char key_asm;
extern unsigned char keyboard_read_ass();

#ifdef LAB3
int sys_inb_cnt(port_t port, unsigned long *byte);
#else
#define sys_inb_cnt(p,q) sys_inb(p,q)
#endif
