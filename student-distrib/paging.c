#include "types.h"
#include "paging.h"

/* 0-4MB has 4KB pages
 * 4-8MB has
 */

//one page is 4kb
//video memory is 4kb page
//one page table is 4mb
//one page directory is 4gb

/*
 * init_paging
 *   DESCRIPTION: initializes paging
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: sets up page directories and page tables
 */
void init_paging()
{
	/* Initialize all entries in the page directory as not present, and
	 * map the pages in the page table to the beginning of memory
	 * with the correct attributes. */
	int i;
	for(i=0; i<ONE_KB; i++) {
		page_directory[i] = NOT_PRESENT;
		first_page_table[i] = (i*FOUR_KB) | NOT_PRESENT;
	}

	/* Put the page table in the page directory */
	page_directory[0]= ((uint32_t)first_page_table) | S_RW_PRESENT;

	/* Setting video memory attributes */
	first_page_table[VIDEO_MEM >> SHIFT12] = VIDEO_MEM | S_RW_PRESENT;
	//page_directory[1]= FOUR_MB | S_RW_PRESENT ;
	page_directory[1]= FOUR_MB |0x83;

	enable_4MB_Paging();
	enable_paging();

	
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
void enable_paging()
{
    asm volatile (
		"movl $page_directory, %%eax;"	/* move the address of the page directory into CR3 indirectly through eax */
		"movl %%eax, %%cr3;"

    	"xorl %%eax, %%eax;"
    	"movl %%cr0, %%eax;"
    	"orl  $0x80000000, %%eax;"	 	/* sets bit 31 of register cr0 to enable paging */
    	"movl %%eax, %%cr0;"
    	:
        :
        : "eax"
    );

}

/* enable_4MB_Paging
 * Inputs: none
 * Return Value: None
 * Function: sets bit 4 of register cr4 to enable 4MB page sizes for the kernel
 */
void enable_4MB_Paging()
{
    asm volatile (
    	"movl %%cr4, %%eax;"
    	"orl  $0x00000010, %%eax;"
    	"movl %%eax, %%cr4;"
    	:
        :
        : "eax"
    );
}

