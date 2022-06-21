;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;I8251A -- Console I/O Drivers for Intel 8251A USART
;
;This driver is best suited for use with actual A revision
;or later 8251 USARTs. For old USARTs, use i8251.inc
;
;This driver will still work with older USARTs, but you may
;get a garbage character read in immediately after cold
;start. Cold starts from an already running GWMON-80 may
;produce garbage output.
;
;Set CTLPRT and DATPRT in the customization file.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SETUP -- Prepare the system for running the monitor
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
