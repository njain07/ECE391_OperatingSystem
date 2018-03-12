#ifndef PAGING
#define PAGING

#define ONE_KB 1024
#define FOUR_KB 4096
#define ONE_MB 1048576
#define FOUR_MB 4194304
#define PRESENT 0x00000003 (?)
#define NOT_PRESENT 0x00000002
#define WRITE (?)
#define USER (?)
#define KERNEL 0x400000


void init_paging();
extern void enable_paging();
void create4MBpage();
void create4KBpage();

#endif