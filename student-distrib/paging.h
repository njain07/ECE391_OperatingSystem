#ifndef PAGING
#define PAGING

#define ONE_KB 1024
#define FOUR_KB ONE_KB*4
#define ONE_MB ONE_KB*ONE_KB
#define FOUR_MB ONE_MB*4
#define MB_128 0x08000000 
#define SHIFT12 12

#define USER (?)
#define S_RW_PRESENT 0x00000003 // attributes: supervisor level, read/write, present
#define NOT_PRESENT 0x00000002
#define U_W_PRESENT 0x00000087 // attributes: user level, write, present

#define KERNEL_VIRTUAL_ADDR 0x400000
#define VIDEO_MEM 0xB8000

// alignment
int first_page_table[ONE_KB] __attribute__((aligned (FOUR_KB)));
int page_directory[ONE_KB] __attribute__((aligned (FOUR_KB)));
//int video_memory[ONE_KB] __attribute__((aligned (FOUR_KB)));


void init_paging();
//void create4MBpage();
//void create4KBpage();
//void enable_4MB_Paging();
void enable_paging();
#endif
