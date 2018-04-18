#include "pcb.h"

file_ops_t rtc_ops = {rtc_open, rtc_read, rtc_write, rtc_close}
file_ops_t dir_ops = {dir_open, dir_read, dir_write, dir_close}
file_ops_t file_ops = {file_open, file_read, file_write, file_close}

