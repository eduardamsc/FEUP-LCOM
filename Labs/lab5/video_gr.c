#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"
#include "video_gr.h"

/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */

#define VRAM_PHYS_ADDR	0xE0000000
#define H_RES             1024
#define V_RES		  768
#define BITS_PER_PIXEL	  8

/* Private global variables */

static void *video_mem; /* Process (virtual) address to which VRAM is mapped */

static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static unsigned vram_size;

void paint_pixel(unsigned short x, unsigned short y, unsigned long color) {
	char* ptr = video_mem;
	*(ptr + x + y * h_res) = color;
}

void *vg_init(unsigned short mode) {
	struct reg86u reg86;

	reg86.u.b.intno = INTERRUPT_VBE;

	reg86.u.b.ah = CALL_VBE;
	reg86.u.b.al = SET_MODE_VBE;
	reg86.u.w.bx = (mode | SET_LINEAR_FRAMEBUFFER);
	if (sys_int86(&reg86) != OK) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return NULL;
	}

	vbe_mode_info_t* vbe_mode = malloc(sizeof(vbe_mode_info_t));

	if (vbe_get_mode_info(mode, vbe_mode) != OK) {
		printf("vg_init(): vbe_get_mode_info failed\n");
		return NULL;
	}

	h_res = vbe_mode->XResolution;
	v_res = vbe_mode->YResolution;
	bits_per_pixel = vbe_mode->BitsPerPixel;
	vram_size = h_res * v_res * bits_per_pixel;

	/* Allow memory mapping */

	int r;
	struct mem_range mr;

	mr.mr_base = (phys_bytes) vbe_mode->PhysBasePtr;
	mr.mr_limit = mr.mr_base + vram_size;

	if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Map memory */

	video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

	if (video_mem == MAP_FAILED)
		panic("couldn't map video memory");
	return video_mem;
}


int vg_exit() {
	struct reg86u reg86;

	reg86.u.b.intno = INTERRUPT_VBE; /* BIOS video services */

	reg86.u.b.ah = SET_VIDEO_MODE; /* Set Video Mode function */
	reg86.u.b.al = TEXT_MODE; /* 80x25 text mode*/

	if (sys_int86(&reg86) != OK) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	} else
		return 0;
}

int check_coordinates(unsigned short x, unsigned short y) {
	if (x < h_res && y < v_res) return 0;
	else return 1;
}
