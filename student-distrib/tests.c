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
<<<<<<< HEAD
void rtc_test(){
    clear();
    rtc_interrupt_flag = 1;
    rtc_open(NULL);
}

/*
void rtc_test2(int* frequency){
    clear();
    rtc_interrupt_flag = 1;
    
    while(1){
        if(rtc_count == 0){
            rtc_open(NULL);
            rtc_write(0,frequency,4);
        }
        
        if(*frequency >= 1024)
            break;
        
        if( (rtc_count % 80) == 0){
            char c[1] = {'1'};
            term_write(0,(char*)c,1);
            *frequency *= 2;
            rtc_write(0,frequency,4);
        }
    }
}
*/

void rtc_test3(){
    rtc_interrupt_flag = 1;
    clear();
    rtc_open(NULL);
    rtc_read(0,0,0);
    
}


=======
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

>>>>>>> 1cc7ccdb38d318f347caee43c56eda1b451b8067
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
