;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;TMS5501 -- Console I/O Drivers for Texas Instruments 
;           TMS 5501 I/O Controller 
;
;Set TMSBASE and SPEED in the customization file.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;TMS 5501 Register Equates
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
STATPRT	equ	TMSBASE		;TMS 5501 status register
BITRATE	equ	TMSBASE		;TMS 5501 bitrate register
DATPRT	equ	TMSBASE+1	;TMS 5501 receive and transmit registers
CMDPRT	equ	TMSBASE+2	;TMS 5501 command register
INTMSK	equ	TMSBASE+3	;TMS 5501 interrupt mask register

TBE	equ	080H		;Mask for Transmitter Buffer Empty
RDA	equ	040H		;Mask for Reciever Data Available

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;TMS 5501 Bitrate Table
;
;Set SPEED equal to one of these values in the customization
;to select bitrate.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
B9600	equ	040H
B4800	equ	020H
B2400	equ	010H
B1200	equ	008H
B300	equ	004H
B150	equ	002H
B110	equ	001H

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;IOSET -- I/O specific setup routine
;
;Resets the TMS 5501, sets the desired bitrate, one stop
;bit, and unmasks all interrupts. Interrupts are disabled.
;
;pre: none
;post: console serial channel is initialized
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
IOSET:	MVI	A, 01H		;Reset TMS 5501
	OUT	CMDPRT
	MVI	A, 080H+SPEED	;Selected speed, 1 stop bit
	OUT	BITRATE
	MVI	A, 0FFH		;Unmask all interrupt sources
	OUT	INTMSK
	RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;CINNE -- Get a char from the console, no echo
;
;If the CANCEL character is received, do a warm start.
;
;pre: console device is initialized
;post: received char is in A register
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CINNE:	IN	STATPRT
	ANI	RDA		;Mask off RDA
	JZ	CINNE		;Zero, wait for data
	IN	DATPRT		;Nonzero, character available
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
COUT1:	IN	STATPRT
	ANI	TBE		;Mask off TBE
	JZ	COUT1		;Not empty, wait
	POP	PSW		;Empty, restore A
	OUT	DATPRT		;Send the character
	RET
