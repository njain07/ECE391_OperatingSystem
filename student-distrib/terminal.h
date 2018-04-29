#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include "types.h"
#include "keyboard.h"
#include "lib.h"
#include "lib.h"
#include "keyboard.h"
#include "syscalls.h"

#define  BUFFER_SIZE    128

#define  READ           0
#define  READ_DONE      1
#define  WRITE          2
#define  WAIT           3

#define NUM_OF_TERMINALS	3

int32_t terminal_num;

/* terminal data structure for multiple terminals */
typedef struct terminal_t
{
	int initialized;
    
    int32_t terminal_processes_array[6];

    uint8_t x_pos;
    uint8_t y_pos;

} terminal_t;

extern terminal_t terminal1;
extern terminal_t terminal2;
extern terminal_t terminal3;

extern terminal_t terminal_array[NUM_OF_TERMINALS];

/* initialize terminal */
void terminal_init(void);

/* Terminal System Calls */
int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes);
int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t terminal_open(const uint8_t* filename);
int32_t terminal_close(int32_t fd);

#endif
