#ifndef PAGING
#define PAGING

#define ONE_KB 1024
#define FOUR_KB ONE_KB*4
#define ONE_MB ONE_KB*ONE_KB
#define FOUR_MB ONE_MB*4
#define MB_128 0x08000000 
#define MB_132 0x08400000
#define SHIFT12 12
#define SHIFT22 22
#define EIGHT_MB FOUR_MB*2

// #define USER (?)
#define S_RW_PRESENT 0x00000003 // attributes: supervisor level, read/write, present
#define NOT_PRESENT 0x00000002
#define U_W_PRESENT 0x00000087 // attributes: user level, write, present
#define U_RW_PRESENT 0x00000007 // not sure what the attributes are; used in vidmap_page

#define KERNEL_VIRTUAL_ADDR 0x400000
#define VIDEO_MEM 0xB8000

// alignment
int first_page_table[ONE_KB] __attribute__((aligned (FOUR_KB)));
int page_directory[ONE_KB] __attribute__((aligned (FOUR_KB)));
int video_memory[ONE_KB] __attribute__((aligned (FOUR_KB)));

/* saving each terminal video memory */
int* terminal_vidmem_array[3];
int terminal1[ONE_KB] __attribute__((aligned (FOUR_KB)));
int terminal2[ONE_KB] __attribute__((aligned (FOUR_KB)));
int terminal3[ONE_KB] __attribute__((aligned (FOUR_KB)));

void paging_init();
//void create4MBpage();
//void create4KBpage();
//void enable_4MB_Paging();
void enable_paging();
void process_page(int32_t pid);
void vidmap_page(uint8_t** screen_start);
void terminal_vidmem(int32_t terminal_num, int32_t new_terminal_num);
void flush_TLB(void);
#endif
