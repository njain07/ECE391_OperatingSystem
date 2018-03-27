#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "rtc.h"

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

void zero_error(){
	TEST_HEADER;

	int a = 1/0;

}

// int idt_pf_test(){
// 	TEST_HEADER;

	
// }

// int idt_mf_test(){
// 	TEST_HEADER;

	
// }
void fault_paging_test()
{
	int* a = 0x4000;
    int* b = 0x7FFFFFD;
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
void rtc_test4(){
	clear();
		int32_t buf[5] = {1, 2, 4, 8, 16};
	rtc_init();

	rtc_write(0, buf[0] ,4);
	rtc_write(0, buf[1] ,4);
	rtc_write(0, buf[2] ,4);
	rtc_write(0, buf[3] ,4);
	rtc_write(0, buf[4] ,4);

}

/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */
void rtc_test()
{
	clear();
	rtc_open(NULL); 
}

/* Test suite entry point */
void launch_tests(){
	TEST_OUTPUT("idt_test", idt_test());
	//while (1);
	// launch your tests here
	 //checkpoint1
	 //zero_error();
	 //fault_paging_test();
	 //no_fault_paging_test();

	 //checkpoint2
	 rtc_test();
}
