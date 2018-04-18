.global execute, halt, open, read, write, close #, getargs, vidmap, set_handler, sigreturn

jmp_table:
	.long execute, halt, open, read, write, close #, getargs, vidmap, set_handler, sigreturn 

# execute:

# halt:

# open:

# read:

# write:

# close:

# getargs:

# vidmap:

# set_handler:

# sigreturn:

common_syscall:
	pushal
	call *jmp_table(, %eax, 4)
	popal
