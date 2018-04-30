#include "types.h"
#include "paging.h"

int32_t terminal_num;

/* 0-4MB has 4KB pages
 * 4-8MB has
 */

//one page is 4kb
//video memory is 4kb page
//one page table is 4mb
//one page directory is 4gb2

/*
 * init_paging
 *   DESCRIPTION: initializes paging
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: sets up page directories and page tables
 */
void paging_init(void)
{
	/* Initialize all entries in the page directory as not present, and
	 * map the pages in the page table to the beginning of memory
	 * with the correct attributes. */
	int i;
	for(i=0; i<ONE_KB; i++) 
	{
		page_directory[i] = NOT_PRESENT;
		first_page_table[i] = (i*FOUR_KB) | NOT_PRESENT;
	}

	/* Put the page table in the page directory */
	page_directory[0]= ((uint32_t)first_page_table) | S_RW_PRESENT;

	/* Setting video memory attributes */
	first_page_table[VIDEO_MEM >> SHIFT12] = VIDEO_MEM | S_RW_PRESENT;
	page_directory[1]= FOUR_MB | S_RW_PRESENT_SIZE;

	enable_paging();

	terminal_vidmem_array[0] = terminal1_vidmem;
	terminal_vidmem_array[1] = terminal2_vidmem;
	terminal_vidmem_array[2] = terminal3_vidmem;
}

/*
 * enable_paging
 *   DESCRIPTION: initializes paging
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: sets CR0 and CR3
 * 				   CR0 - the 32nd bit of this register holds the paging bit; we set it to 1 to enable paging
 * 				   CR3 - holds the address of the page directory; used when virtual addressing is enabled
 */
void enable_paging(void)
{
    asm volatile (
		"movl %0, %%eax;"			/* move the address of the page directory into CR3 indirectly through eax */
		"movl %%eax, %%cr3;"
    	"movl %%cr4, %%eax;"
    	"orl  $0x00000010, %%eax;"
    	"movl %%eax, %%cr4;"
    	"xorl %%eax, %%eax;"
    	"movl %%cr0, %%eax;"
    	"orl  $0x80000000, %%eax;"	 /* sets bit 31 of register cr0 to enable paging */
    	"movl %%eax, %%cr0;"
    	:
        :"r"(page_directory)
        :"eax"
    );

}

void process_page(int32_t pid)
{
	int32_t user_loc = EIGHT_MB + (pid * FOUR_MB);
	page_directory[32] = user_loc | U_W_PRESENT;
	flush_TLB();
}

void vidmap_page(uint8_t** screen_start)
{
	video_memory[0] = VIDEO_MEM | U_RW_PRESENT;
	page_directory[33] = (uint32_t)video_memory | U_RW_PRESENT;
	flush_TLB();
}

void terminal_vidmem(int32_t terminal_num, int32_t new_terminal_num)
{
	// memcpy current video memory into terminal_num's video memory
	memcpy((int*)terminal_vidmem_array[terminal_num-1], (int*)VIDEO_MEM, FOUR_KB);
	clear();
    // memcpy new_terminal_num's video memory into current video memory
    memcpy((int*)VIDEO_MEM, (int*)terminal_vidmem_array[new_terminal_num-1], FOUR_KB);
}

void flush_TLB(void)
{
	asm volatile (
		"movl %%cr3, %%eax;"
		"movl %%eax, %%cr3;"
		:
		:
		: "eax"
	);
}

