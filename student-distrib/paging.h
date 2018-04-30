#ifndef PAGING
#define PAGING

#include "lib.h"

#define ONE_KB 		1024
#define FOUR_KB 	ONE_KB*4
#define ONE_MB 		ONE_KB*ONE_KB
#define FOUR_MB 	ONE_MB*4
#define MB_128 		0x08000000 
#define MB_132 		0x08400000
#define SHIFT12 	12
#define SHIFT22 	22
#define EIGHT_MB 	FOUR_MB*2

#define S_RW_PRESENT 		0x00000003 // attributes: supervisor level, read/write, present
#define NOT_PRESENT 		0x00000002
#define S_RW_PRESENT_SIZE 	0x00000083
#define U_W_PRESENT 		0x00000087 // attributes: user level, write, present
#define U_RW_PRESENT 		0x00000007 // not sure what the attributes are; used in vidmap_page

#define KERNEL_VIRTUAL_ADDR 0x400000
#define VIDEO_MEM 			0xB8000

#define NUM_OF_TERMINALS 	3

// alignment
int first_page_table[ONE_KB] __attribute__((aligned (FOUR_KB)));
int page_directory[ONE_KB] __attribute__((aligned (FOUR_KB)));
int video_memory[ONE_KB] __attribute__((aligned (FOUR_KB)));

/* saving each terminal video memory */
int* terminal_vidmem_array[NUM_OF_TERMINALS];
int terminal1_vidmem[FOUR_KB]; // __attribute__((aligned (FOUR_KB)));
int terminal2_vidmem[FOUR_KB]; // __attribute__((aligned (FOUR_KB)));
int terminal3_vidmem[FOUR_KB]; // __attribute__((aligned (FOUR_KB)));

void paging_init(void);
void enable_paging(void);
void process_page(int32_t pid);
void vidmap_page(uint8_t** screen_start);
void terminal_vidmem(int32_t terminal_num, int32_t new_terminal_num);
void flush_TLB(void);
#endif
