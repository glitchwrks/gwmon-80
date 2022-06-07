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
IOSET:	MVI	A, 040H
	OUT	CTLPRT
	MVI	A, 04EH
	OUT	CTLPRT
	MVI	A, 037H
	OUT	CTLPRT
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
