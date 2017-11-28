#include "i8042.h"

.global _keyboard_read_ass

.global _key_asm

.data
_key_asm:
.byte 0

.text
_keyboard_read_ass:
mov $NTRIES, %ecx
ciclo:
xor %eax, %eax
inb $STAT_REG /* resultado para al */
testb $OBF, %al
jz continua
testb $TIMEOUT, %al
jnz erro
testb $PARITY, %al
jnz erro
inb $OUT_BUF
mov %al, _key_asm
jmp fim
continua:
loopnz ciclo
erro:
mov $1, %al
mov %al, _key_asm
fim:
ret
