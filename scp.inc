;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SCP -- GWMON-80 Small Command Processor
;
;This is the small command processor for Glitch Works
;Monitor for 8080/8085/Z80 and compatible.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;CMDLP -- Small command processor loop
;
;Get a character from the console device and immediately 
;handle it by passing off to helper function.
;
;Falls through to ERROR if an invalid command is specified.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CMDLP:	LXI	H, WSTART	;HL = warm start address
	PUSH	H		;Prime stack for RET
	LXI	H, PROMPT$
	CALL	PRTCLS
	CALL	CIN
	ORI	20H		;Downcase input

	LXI	H, CMDTAB - 1
CMDLP1:	INX	H		;Point to next command char
	CMP	M
	JZ	RUNCMD		;Match, run command handler
	MOV	B, A
	MOV	A, M
	ORA	A
	MOV	A, B
	INX	H		;Get past handler address
	INX	H
	JNZ	CMDLP1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;ERROR -- Print generic error message and abort
;
;Falls through to PRTERR.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
ERROR:	LXI	H, ERR$		;Fall through to PRTERR

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;PRTERR -- Print a null-terminated error string
;
;pre: HL contains pointer to start of null-terminated string
;post: string at HL printed to console
;post: program execution returned to command loop
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
PRTERR:	CALL	PRTCLS
	JMP	WSTART		;Warm start, restore SP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;RUNCMD -- Run a command handler from CMDTAB
;
;pre: H register points to command letter
;post: control transferred to command handler
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
RUNCMD:	CALL	PRTSPC
	INX	H		;Point to low handler address
	MOV	E, M
	INX	H
	MOV	D, M
	XCHG
	PCHL

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;GETADR -- Get a 16-bit address from the console
;
;pre: none
;post: HL contains address from console
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
GETADR:	CALL	GETHEX
	MOV	H, A
	CALL	GETHEX
	MOV	L, A
	RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;GETHEX -- Get byte from console as hex
;
;Enter at GETHE2 with character in A register from EDTCMD.
;
;pre: none
;post: A register contains byte from hex input
;post: Carry flag set if non hex character received
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
GETHEX:	CALL 	CIN
GETHE2:	PUSH	H
	MOV	H, A
	CALL	CIN
	MOV	L, A
	CALL	HEXBYT
	POP	H
	JC	ERROR		;Invalid hex char, abort
	RET
