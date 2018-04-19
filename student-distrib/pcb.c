#include "pcb.h"

file_ops_t rtc_ops = {rtc_read, rtc_write, rtc_open, rtc_close};
file_ops_t dir_ops = {dir_read, dir_write, dir_open, dir_close};
file_ops_t file_ops = {file_read, file_write, file_open, file_close};
file_ops_t terminal_ops = {terminal_read, terminal_write, terminal_open, terminal_close};

