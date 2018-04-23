#include "tests.h"

#define PASS 1
#define FAIL 0

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}


/* Checkpoint 1 tests */

/* IDT Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0; i < 10; ++i){
		if ((idt[i].offset_15_00 == NULL) && 
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}

	return result;
}

// void zero_error()
// {
// 	TEST_HEADER;
// 	int a = 1/0;
// }

// int idt_pf_test()
// {
// 		TEST_HEADER;
// }

// int idt_mf_test()
// {
// 		TEST_HEADER;
// }

void fault_paging_test()
{
	int* a = (int*) 0x4000;
    int* b = (int*) 0x7FFFFFD;
    *a = 0;
    *b = 0;
}
void no_fault_paging_test()
{
	TEST_HEADER;
	
	//should not page fault
	int * var;
	
	//tests kernel
	var = (int *)0x400000;
	*var = *var;
	
	var = (int *)0x400001;
	*var = *var;
	
	var = (int *)0x799999;
	*var = *var;
	
	//tests vid mem
	var = (int *)0xB8000;
	*var = *var;
	
	var = (int *)0xB8001;
	*var = *var;
	
	var = (int *)0xB8999;
	*var = *var;
}

/* Checkpoint 2 tests */

void rtc_test()
{
    clear();
    rtc_interrupt_flag = 1;
    rtc_open(NULL);
}


// void rtc_test2(int* frequency)
// {
//     clear();
//     rtc_interrupt_flag = 1;
    
//     while(1){
//         if(rtc_count == 0){
//             rtc_open(NULL);
//             rtc_write(0,frequency,4);
//         }
        
//         if(*frequency >= 1024)
//             break;
        
//         if( (rtc_count % 80) == 0){
//             char c[1] = {'1'};
//             term_write(0,(char*)c,1);
//             *frequency *= 2;
//             rtc_write(0,frequency,4);
//         }
//     }
// }

void rtc_test3()
{
    rtc_interrupt_flag = 1;
    clear();
    rtc_open(NULL);
    rtc_read(0,0,0);
    
}

// void rtc_test4()
// {
// 	clear();
// 		int32_t buf[5] = {1, 2, 4, 8, 16};
// 	rtc_init();

// 	rtc_write(0, (void*)buf[0] ,4);
// 	rtc_write(0, (void*)buf[1] ,4);
// 	rtc_write(0, (void*)buf[2] ,4);
// 	rtc_write(0, (void*)buf[3] ,4);
// 	rtc_write(0, (void*)buf[4] ,4);
// }

void terminal_test()
{
	int32_t n;
	char c_array[120];
	n = terminal_read(0, c_array, 0);
	// printf("terminal_read: %s\n", c_array);

	// char c_array[120] = "hello";
	terminal_write(0, c_array, n);
}

void filesys_list_of_files()
{
	//call dir_read many times to print all files
	
}

void filesys_file_content(uint32_t dentry_index)
{
	// tests read_dentry_by_index and read_data
	//loop across inodes to print files
	dentry_t* dentry;
	uint32_t inode_index, nbytes;
	uint8_t buf[1025];
	read_dentry_by_index(dentry_index, dentry);
	if(dentry->file_type == 2)
	{
		clear();
		inode_index = dentry->inode_num;
		// nbytes = // how to find length of file?
		nbytes = 1025;
		read_data(inode_index, 0, buf, nbytes);
		printf("%s\n", buf);
		// write(1, buf, nbytes);
		// printf("returned successfully\n");
	}
	return;
}

void filesys_read_dentry_by_name(uint8_t* fname)
{
	//tests file_open and dir_open
	dentry_t* dentry;
	// printf("I reached here\n");
	read_dentry_by_name(fname, dentry);
	clear();
	printf("File Name: %s\n", dentry->file_name);
	printf("File Type: %d\n", dentry->file_type);
	printf("Inode Num: %d\n", dentry->inode_num);
}

/* Checkpoint 3 tests */

void open_close_syscall(uint8_t* filename)
{
	int32_t fd; //, bytes_printed;
	// int8_t* buf = "hello";
	fd = open(filename);
	// printf("FD: %d\n", fd);
	// bytes_printed = write(fd, (void*)buf, 5);
	// printf("Bytes Printed: %d\n", bytes_printed);
	close(fd);
}

/* Checkpoint 4 tests */
/* Checkpoint 5 tests */

/* Test suite entry point */
void launch_tests(){
	//TEST_OUTPUT("idt_test", idt_test());
	//while (1);
	// launch your tests here

	//checkpoint1
	//zero_error();
	//fault_paging_test();
	//no_fault_paging_test();	

	//checkpoint2	
	// int *ptr;	
	// int a= 2;	
	// ptr = &a;	
	// printf("A");
	// rtc_set_int_freq(2);
	// while(1<2)
	// {
	// 		printf("1");
	// 		rtc_read(0,ptr,4);		
	// 		printf("2");
	// }

	// terminal_test();

	// filesys_list_of_files();
	filesys_file_content(16);
	// filesys_read_dentry_by_name((uint8_t*)"verylargetextwithverylongname.tx");

	//checkpoint 3
	// open_close_syscall((uint8_t*)"hello");
}
