#include "types.h"
#include "paging.h"

/* 0-4MB has 4KB pages
 * 4-8MB has
 */

//one page is 4kb
//video memory is 4kb page
//one page table is 1kb
//one page directory is 1kb


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
	first_page_table[VIDEO_MEM/FOUR_KB] = first_page_table[VIDEO_MEM/FOUR_KB] | S_RW_PRESENT;
	page_directory[1]= FOUR_MB | S_RW_PRESENT | FOUR_MB;

	enable_4MB_Paging();

	enable_paging();

	
}

/* enable_paging()
 * 
 * Sets CR0, CR3, and CR4
 * CR0 - the 32nd bit of this register holds the paging bit; we set it to 1 to enable paging
 * CR3 - holds the address of the page directory; used when virtual addressing is enabled
 * CR4 - used in protected mode to enable page size extension; determines whether the IF flag can be enabled 
 */
extern void enable_paging()
{
	asm volatile (
		"xorl %%eax, %%eax;"
		"movl $page_directory, %%eax;"	//move the address of the page directory into CR3 indirectly through eax
		"movl %%eax, %%cr3;"
		"xorl %%eax, %%eax;"			//clear eax
		"movl %%cr4, %%eax;"
		"orl $0x00000010, %%eax;"		//enable page size extension (PAE) in CR4 indirectly through eax 
		"movl %%eax, %%cr4;"
		"xorl %%eax, %%eax;"				//clear eax
		"movl %%cr0, %%eax;"
		"orl $0x80000001, %%eax;"		//enable the paging bit in CR0 indirectly through eax
		"movl %%eax, %%cr0;"
		:								//input regs
		:								//output regs
		:"eax"							//clobbered regs
		);
}

/* create_process_page()
 * 
 * Creates a 4MB page which maps the virtual address at 128MB to the corresponding physical memory
 */
void create_process_page()
{
	// 
	//page_directory[32] = 

	// Flush TLB
	asm volatile (
		"movl %%cr3, %%eax;"
		"movl %%eax, %%cr3;"
		:								//input regs
		:								//output regs
		:"eax"							//clobbered regs
		);
}



/* enablePaging
 * Inputs: none
 * Return Value: None
 * Function: sets bit 31 of register cr0 to enable paging */
static inline void enablePaging()
{
    asm volatile (
    	"xorl %%eax, %%eax;"
		"movl $page_directory, %%eax;"	//move the address of the page directory into CR3 indirectly through eax
		"movl %%eax, %%cr3;"

    	"mov %%cr0, %%eax;"
    	"or  $0x80000000, %%eax;"	 	/* sets bit 31 of register cr0 to enable paging */
    	"mov %%eax, %%cr0;"
    	:
        :
        : "eax"
    );
}

/* enable4MBPaging
 * Inputs: none
 * Return Value: None
 * Function: sets bit 4 of register cr4 to enable 4MB page sizes */
static inline void enable_4MB_Paging()
{
    asm volatile (
    	"mov %%cr4, %%eax;"
    	"or  $0x00000010, %%eax;"			/* sets bit 4 of register cr4 to enable 4MB page sizes */
    	"mov %%eax, %%cr4;"
    	:
        :
        : "eax"
    );
}

