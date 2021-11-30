;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;6850ACIA -- Console I/O Drivers for Motorola 6850 ACIA
;
;Set CTLPRT and DATPRT in the customization file.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;IOSET -- I/O specific setup routine
;
;pre: none
;post: console serial channel is initialized
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
IOSET:	MVI	A, 03H
	OUT	CTLPRT		;Reset ACIA
	MVI	A, 15H
	OUT	CTLPRT		;8N1, /16 bitrate
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
	ANI	01H
	JZ	CINNE
	IN	DATPRT
	CPI	CANCEL
	JZ	WSTART
	RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;COUT -- Output a character to the console
;
;pre: A register contains char to be printed
;post: character is printed to the console
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
COUT:	PUSH	PSW		;Save A on stack
COUT1:	IN	CTLPRT
	ANI	02H		;Mask off transmitter status
	JZ	COUT1
	POP	PSW		;Restore A
	OUT	DATPRT
	RET
