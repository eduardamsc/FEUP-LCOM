#include "i8042.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>

int kbc_write(unsigned long cmd);

int kbc_read(unsigned long *data);
