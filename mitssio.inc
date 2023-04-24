;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;MITSSIO -- Console I/O Drivers for MITS 88-SIO Board
;
;Set CTLPRT and DATPRT in the customization file.
;
;The MITS 88-SIO uses a standard 40-pin UART (e.g. TR1402),
;but due to the nature of systems with 40-pin UARTs, it is
;not practical to write a single I/O module for them. The
;system designer can choose how to map status bits, and
;their polarities. Interrupt capabilities, if present, are
;specific to the system or board.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Hardware Equates
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
RRFULL	equ	00100000b	;Receive register full mask
TREMPTY	equ	00000010b	;Transmit register empty mask

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;IOSET -- I/O specific setup routine
;
;pre: none
;post: console serial channel is initialized
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
IOSET:	MVI	A, 03H		;Disable interrupts
	OUT	CTLPRT
	RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;CINNE -- Get a char from the console, no echo
;
;If the CANCEL character is received, do a warm start.
;
;pre: console device is initialized
;post: received char is in A register
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CINNE:	IN	CTLPRT
	ANI	RRFULL
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
COUT:	PUSH	PSW		;Save A on stack
COUT1:	IN	CTLPRT
	ANI	TREMPTY		;Mask off transmitter status
	JZ	COUT1
	POP	PSW		;Restore A
	OUT	DATPRT
	RET
