	/*	errno hack	*/
	.global	___errno

	.align	2
start:
	/*	Clear Status Reg	*/
	AND	#0, r0
	LDC	r0, sr

	/*	Go !	*/
	MOV.L	app_stack_addr, r15
	MOV.L	start_addr, r0
	JSR	@r0
	NOP

        /*      SYSCALL SYS_exit        */
        mov     #1, r4
        trapa   #34


	.align	2
app_stack_addr:	
	.long	(0x8000000 + (1 << 22))

	.align	2
start_addr:
	.long	_startup
