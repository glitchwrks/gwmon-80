;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;CPM80IO -- Console I/O Drivers for CP/M-80 1.4 through 3.0
;
;This I/O module uses BDOS direct console I/O.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;CP/M Equates
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
BDOS	equ	0005H
CONIO	equ	0006H
GETCH	equ	0FFH

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;IOSET -- I/O specific setup routine
;
;pre: none
;post: none
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
IOSET:	RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;CINNE -- Get a char from the console, no echo
;
;If the CANCEL character is received, do a warm start.
;
;pre: console device is initialized
;post: received char is in A register
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CINNE:	PUSH	B
	PUSH	D
	PUSH	H
CINNE1:	MVI	E, GETCH
	MVI	C, CONIO
	CALL	BDOS
	ORA	A
	JZ	CINNE1
	CPI	CANCEL
	JZ	WSTART
	POP	H
	POP	D
	POP	B
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
COUT:   PUSH	B
	PUSH	D
	PUSH	H
	PUSH	PSW
	MVI	C, CONIO
	MOV	E, A
	CALL	BDOS
	POP	PSW
	POP	H
	POP	D
	POP	B
	RET
