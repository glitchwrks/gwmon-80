;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SCMDSTD -- Small Monitor Standard Commands
;
;This file contains implementations of all standard SM
;commands. The command table is located at the end of the
;file.
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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;EDTCMD -- Edit memory command
;
;Breaks out to main command loop through GETHEX jump to
;WSTART.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
EDTCMD:	CALL	GETADR		;HL = address to open
EDTCM1:	CALL	PRTADR		;Print location address
	CALL	PRTSPC
	CALL	DMPLOC		;Print contents of location
	CALL	PRTSPC
	CALL	CIN		;Get user input
	CPI	NEXTLOC		;Check for NEXTLOC character
	JZ	EDTCM2		;Yes, go to next location
	CALL	GETHE2		;No, process as hex input
	MOV	M, A
	CALL	PRTSPC
	CALL	DMPLOC		;Print contents of location (verification)
EDTCM2:	INX	H
	JMP	EDTCM1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;GOCMD -- Call out to a specified address
;
;The stack is primed with the GWMON-80 warm start address.
;Called program can return control to GWMON-80 through a RET
;as long as the stack is not disturbed.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
GOCMD:	CALL	GETADR		;HL = Address to call
	PCHL			;Go to specified address

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;INPCMD -- Input to port command
;
;Returns through PRTHEX.
;
;pre: DYNIN memory area has been initialized
;post: byte from specififed port printed to console
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
INPCMD:	CALL	GETHEX
	STA	DYNIN + 1	;Store operand
	CALL	PRTSPC
	CALL	DYNIN
	JMP	PRTHEX

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;LODCMD -- Load an Intel HEX file from console
;
;This loader accepts data with both CR (*NIX) and
;CR,LF (DOS/Windows) terminated lines.
;
;Intel HEX record may be ended with:
;
;   * DATA (0x00) record with length of 0
;   * Any nonzero record type
;
;post: Intel HEX file loaded, or error printed
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
LODCMD:	CALL	CRLF
LODCM1:	CALL	CINNE
	CPI	':'
	JNZ	LODCM1		;Wait for start colon
	CALL	COUT		;Print start colon
	CALL	GETHEX		;Get record length
	JZ	LODCM4		;Length == 0, done
	MOV	B, A		;Record length in B
	MOV	C, A		;Start checksumming in C

	CALL	GETADR		;HL = 16-bit starting address
	ADD	C		;A == L from GETADR
	ADD	H
	MOV	C, A		;Checksum

	CALL	GETHEX		;Get record type
	JNZ	LODCM4		;Not Record Type 00 (DATA), done

LODCM2:	CALL	GETHEX		;This is the main record processing loop
	MOV	M, A		;Store char at HL
	ADD	C
	MOV	C, A		;Checksum
	INX	H		;Move memory pointer up
	DCR	B
	JNZ	LODCM2		;Not done with the line
LODCM3:	CALL	GETHEX		;Get checksum byte
	ADD	C
	JNZ	CSUMER		;Checksum bad, print error
	JMP	LODCMD		;Process more records
LODCM4:	CALL	CIN		;Done getting data, eat chars
	CPI	LF
	JNZ	LODCM4		;No LF, keep eating
	RET			;Got LF, return to command loop
CSUMER:	LXI	H, CSERR$	;Print checksum error to console
	JMP	PRTERR		;RET from ERROUT will return to command loop

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;OUTCMD -- Output to port command
;
;Returns through DYNOUT.
;
;pre: DYNOUT memory area has been initialized
;post: specified byte has been written to specified port
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
OUTCMD:	CALL	GETHEX
	STA	DYNOUT + 1	;Store operand
	CALL	PRTSPC
	CALL	GETHEX
	JMP	DYNOUT

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;CMDTAB -- Table/array of commands
;
;This data *must* be the last item in SCMDSTD.INC to allow
;chaining of additional command tables.
;
;Table entry structure:
;    * Single command character, lowercase only
;    * Pointer to implementation routine
;
;The last entry should contain 0x00 for the command char and
;no additional address. It is provided in SCMDNULL.INC
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CMDTAB:	db	'd'
	dw	HDCMD
	db	'e'
	dw	EDTCMD
	db	'g'
	dw	GOCMD
	db	'i'
	dw	INPCMD
	db	'l'
	dw	LODCMD
	db	'o'
	dw	OUTCMD
