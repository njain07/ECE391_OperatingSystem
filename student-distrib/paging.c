#include "paging.h"

/* 0-4MB has 4KB pages
 * 4-8MB has
 */

//one page is 4kb
//video memory is 4kb page
//one page table is 1kb
//one page directory is 1kb

#define ONE_KB 1024
#define FOUR_KB ONE_KB*4
#define ONE_MB ONE_KB*ONE_KB
#define FOUR_MB ONE_MB*4

#define USER (?)
#define S_RW_PRESENT 0x00000003 // attributes: supervisor level, read/write, present
#define NOT_PRESENT 0x00000002

#define KERNEL_VIRTUAL_ADDR 0x400000

// alignment
int page_table[ONE_KB] __attribute__((aligned (FOUR_KB)));
int page_directory[ONE_KB] __attribute__((aligned (FOUR_KB)));
int video_page_table[ONE_KB] __attribute__((aligned (FOUR_KB)));


void init_paging()
{
	/* Initialize all entries in the page directory as not present, and
	 * map the pages in the page table to the beginning of memory
	 * with the correct attributes. */
	int i;
	for(i=0; i<ONE_KB; i++) {
		page_directory[i] = NOT_PRESENT;
		page_table[i] = (i*FOUR_KB) | S_RW_PRESENT;
	}

	/* Put the page table in the page directory */
	page_directory[0] = ((uint32_t)page_table) | S_RW_PRESENT;

	enable_paging();
}

extern void enable_paging()
{
	/* Sets CR0, CR3, and CR4
	 * CR0 - the 32nd bit of this register holds the paging bit; we set it to 1 to enable paging
	 * CR3 - holds the address of the page directory; used when virtual addressing is enabled
	 * CR4 - used in protected mode to enable page size extension; determines whether the IF flag can be enabled 
	 */
	asm volatile (
		"movl %%eax, %%cr3;"
		"movl %%cr4, %%eax;"
		"orl $0x00000010, %%eax;"
		"movl %%cr0, %%eax;"
		"orl $0x80000001, %%eax;"
		"movl %%eax, %%cr0;"
		:							//input regs
		:							//output regs
		:"eax"						//clobbered regs
		);
}

void create4MBpage()
{
	
}

void create4KBpage()
{

}

