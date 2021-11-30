;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;I8251 -- Console I/O Drivers for Intel 8251 USART
;
;Set CTLPRT and DATPRT in the customization file.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SETUP -- Prepare the system for running the
;   monitor
;
;pre: none
;post: stack and console are initialized
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
IOSET:	LXI	H, INIUART$
	MVI	B, 06H		;Length of USART init string
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
	RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;COUT -- Output a character to the console
;
;pre: A register contains char to be printed
;post: character is printed to the console
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
COUT:	PUSH	PSW		;Save char to print on stack
COUT1:  IN	CTLPRT
	ANI	01H
	JZ	COUT1
	POP	PSW		;Restore char to print
	OUT	DATPRT
	RET

;Init string for the 8251, x16 clock, 8N1
INIUART$:  db 00H, 00H, 00H, 40H, 4EH, 37H