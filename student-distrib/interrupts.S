.global interrupt_rtc 
# Ok so we need to write assembly code to ensure 
interrupt_rtc:
		pushal
		call rtc_interrupt_handler
		popal
		iret
		