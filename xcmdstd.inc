;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;XCMDSTD -- Extended Monitor Standard Commands
;
;This file defines the linked list of basic commands
;supported by the XM and the routines that implement them.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;HDCMD -- Hex dump command
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
HDCMD:	CALL	GETADR		;HL = start address
	XCHG			;DE = start address
	CALL	PRTSPC
	CALL	GETADR		;HL = end address
	XCHG			;HL = start, DE = end
	JMP	HEXDMP		;Return through HEXDMP

EDTCMD: HLT

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
PRTERR:	CALL	PRTSTR
	JMP	WSTART		;Warm start, restore SP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;CMDLST -- Linked list of commands
;
;This data *must* be the last item in XCMDSTD.INC to allow
;chaining of additional command lists.
;
;List entry structure:
;    * Command string, NULL-terminated
;    * Pointer to implementation routine
;    * Pointer to next entry
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CMDLST:	
DUMP:	db 'DUMP', 0
	dw HDCMD
	dw EDIT

EDIT:	db 'EDIT', 0
	dw EDTCMD