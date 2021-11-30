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