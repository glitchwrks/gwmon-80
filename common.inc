;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;COMMON -- Functions Common to SM and XM
;
;These functions are used by both the SM and XM versions
;of GWMON-80.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;ASCII Equates
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CTLC	equ	03
BS	equ	08
LF	equ	10
CR	equ	13

CANCEL	equ	CTLC		;Escape/cancel character, CTRL+C
NEXTLOC	equ	CR		;Keystroke for next location in E
				;defaults to CR

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;HEXBYT -- Convert pair of ASCII characters to byte
;
;pre: H register contains high ASCII coded hex nybble
;pre: L register contains low ASCII coded hex nybble
;post: A register contains converted byte
;post: CY flag set if non-hex character input
;post: Z flag set if conversion results in 0x00
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
HEXBYT:	MOV	A, H
	CALL	ASCHEX
	RC
	RLC
	RLC
	RLC
	RLC
	MOV	H, A
	MOV	A, L
	CALL	ASCHEX
	RC
	ORA	H
	RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;ASCHEX -- Convert ASCII character to hex nybble
;
;pre: A register contains ASCII coded nybble
;post: A register contains nybble
;post: CY flag set if ASCII character is invalid hex
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
ASCHEX:	CPI	'0'		;Less than '0'?
	RC			;Yes, CY set, return
	SUI	'0'		;ASCII to decimal bias
	CPI	0AH
	STC			;Set CY to known state
	CMC			;Clear CY
	RM			;0x0 - 0x9, done
	ANI	5FH		;Upcase
	SUI	07H		;ASCII to hex bias
	CPI	10H
	CMC			;Set CY if > 'F'
	RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;HEXDMP -- Hex dump memory to console
;
;pre: HL register pair contains starting address
;pre: DE register pair contains ending address
;post: contents of memory printed to console
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
HEXDMP:	CALL	CRLF
	CALL	DUMP16
	MOV	A, D		;A = high byte of end address
	CMP	H		;Compare current high address
	RC			;Done if current high > end high
	MOV	A, E		;Low byte of end address
	CMP	L		;Compare current low address
	JNC	HEXDMP		;No CY, not done, continue dumping memory
	MOV	A, D		;High byte of end address
	CMP	H		;Are high bytes the same?
	JNZ	HEXDMP		;Keep going if upper bytes aren't the same
	RET			;Done if they are

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;DUMP16 -- Dump 16 consecutive memory locations as hex
;
;Destroys contents of C register.
;
;pre: HL register pair contains starting memory address
;post: memory from HL to HL + 16 printed to console
;post: HL incremented to HL + 16
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
DUMP16:	CALL	ADROUT
	MVI	C, 10H
DUMP11:	CALL	PRTSPC
	CALL	DMPLOC
	INX	H
	DCR	C
	RZ
	JMP	DUMP11

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;DMPLOC -- Print a byte at HL to console
;
;Returns through PRTHEX.
;
;pre: HL pair contains address of byte
;post: byte at HL printed to console
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
DMPLOC:	MOV	A, M
	JMP	PRTHEX

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;ADROUT -- Print an address to the console
;
;Prints a 16-bit address, a space, and a colon. Destroys
;A register contents.
;
;Returns through COUT.
;
;pre: HL pair contains address to print
;post: HL printed to console as hex
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
ADROUT: MOV	A, H
	CALL	PRTHEX
	MOV	A, L
	CALL	PRTHEX
	CALL	PRTSPC
	MVI	A, ':'
	JMP	COUT

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;PRTHEX -- Output byte to console as hex
;
;Falls through to PRTNIB.
;
;pre: A register contains byte to be output
;post: byte is output to console as hex
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
PRTHEX: PUSH	PSW		;Save A register on stack
	RRC			;Rotate high nybble down
	RRC
	RRC
	RRC
	ANI	0FH
	CALL	PRTNIB		;Print high nybble
	POP	PSW		;Restore A register
	ANI	0FH

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;PRTNIB -- Print hex nybble on console
;
;Returns through COUT.
;
;pre: A register contains nybble
;post: nybble printed to console
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
PRTNIB:	ADI	90H
	DAA
	ACI	40H
	DAA
	JMP	COUT

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;PRTSPC -- Print a space to the console
;
;Returns through COUT.
;
;pre: none
;post: ASCII space printed to console
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
PRTSPC:	MVI	A, ' '
	JMP	COUT

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;CRLF -- Print a CR, LF
;
;Returns through COUT.
;
;pre: none
;post: CR, LF printed to console
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CRLF:	MVI	A, CR
	CALL	COUT
	MVI	A, LF
	JMP	COUT

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;PRTSTR -- Print a null-terminated string
;
;Destroys contents of A register.
;
;pre: HL contains pointer to start of null-terminated string
;post: string at HL printed to console
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
PRTSTR:	MOV	A, M
	ORA	A		;Set flags
	RZ			;A == 0x00, done
	CALL	COUT
	INX	H
	JMP	PRTSTR

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Common Monitor Strings
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
PROMPT$:	db CR, LF, LF, '>', 0