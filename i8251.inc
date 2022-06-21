;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;I8251 -- Console I/O Drivers for Intel 8251 USART
;
;This driver will eat garbage from old 8251 USARTs present
;after a device reset. If you don't need this functionality,
;you can save a few bytes by using i8251a.inc
;
;Set CTLPRT and DATPRT in the customization file.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SETUP -- Prepare the system for running the monitor
;
;Once the USART is initialized, check for garbage characters
;that would otherwise flow into the command processor on
;cold start. This is a problem particular to old 8251 USARTs
;and generally does not affect 8251A and later CMOS USARTs.
;
;pre: none
;post: stack and console are initialized
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
IOSET:	LXI	H, INIUART$
	MVI	B, INILEN	;Length of USART init string
IOSET1:	MOV	A, M
	OUT	CTLPRT
	INX	H
	DCR	B
	JNZ	IOSET1
IOSET2:	IN	CTLPRT		;Eat garbage following a reset
	ANI	02H		;Check for char available
	RZ			;Nothing there, done
	IN	DATPRT		;Garbage, eat it
	JMP	IOSET2		;Check for more garbage
	RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;CINNE -- Get a char from the console, no echo
;
;pre: console device is initialized
;post: received char is in A register
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CINNE:	IN	CTLPRT
	ANI	02H
	JZ	CINNE
	IN	DATPRT
	CPI	CANCEL
	JZ	WSTART
	RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;COUT -- Output a character to the console
;
;This routine *must* preserve the contents of the A register
;or CIN will not function properly.
;
;pre: A register contains char to be printed
;post: character is printed to the console
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
COUT:	PUSH	PSW		;Save char to print on stack
COUT1:  IN	CTLPRT
	ANI	01H
	JZ	COUT1
	POP	PSW		;Restore char to print
	OUT	DATPRT
	RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;INIUART$ -- Init string for the 8251 USART
;
;This fixed-length string initializes an 8251 USART from an
;unknown state. Sending three NULLs will ensure command
;mode.
;
;INILEN specifies the length of the initialization string.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
INIUART$:	db 00H, 00H, 00H, 40H, 4EH, 37H
INILEN	equ	* - INIUART$
