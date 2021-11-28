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
CMDLP:	LXI	H, PROMPT$
	CALL	PRTSTR
	CALL	CIN
	ANI	5FH		;Upcase input
	PUSH	PSW		;Save it on stack
	CALL	PRTSPC
	POP	PSW		;Restore command char
	CPI	'D'
	CZ	HDCMD
	CPI	'E'
	CZ	EDTCMD
	CPI	'G'
	CZ	GOCMD
	CPI	'I'
	CZ	INPCMD
	CPI	'L'
	CZ	LODCMD
	CPI	'O'
	CZ	OUTCMD
	CPI	CR
	JZ	CMDLP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;ERROR -- Print generic error message and abort
;
;Returns to the command processing loop. Falls through to
;PRTERR.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
ERROR:	LXI	H, ERR$		;Fall through to PRTERR

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;PRTERR -- Print a null-terminated error string
;
;pre: HL contains pointer to start of null-terminated string
;post: string at HL printed to console
;post: program execution returned to command loop
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
PRTERR:	CALL	CRLF
	CALL	PRTSTR
	JMP	CMDLP

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
EDTCM1:	CALL	CRLF
	CALL	ADROUT		;Print location address
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
;The stack is primed with the GWMON-80 warm start address,
;then the address to call out to. Execution is transferred
;via a RET. Called program can return control to GWMON-80
;through a RET as long as the stack is not disturbed.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
GOCMD:	CALL	GETADR		;HL = Address to call
	LXI	D, WSTART
	PUSH	D		;Prime stack with WSTART address
	PUSH	H		;Push specified address
	RET			;RET to specified address

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
	ADD	C
	MOV	C, A		;Checksum record type
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
GETHEX:	PUSH	H
	CALL 	CIN
GETHE2:	MOV	H, A
	CALL	CIN
	MOV	L, A
	CALL	HEXBYT
	POP	H
	JC	ERROR		;Invalid hex char, abort
	RET
