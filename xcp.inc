;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;XCP -- GWMON-80 Extended Command Processor
;
;This is the extended command processor for Glitch Works
;Monitor for 8080/8085/Z80 and compatible.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;READLN -- Read a line of input from the console.
;
;This rountine reads up to MAXBUF characters from the
;console and stores them in INBUFF. Input is terminated by
;a CR or when MAXBUF is reached.
;
;pre: INBUFF is defined and points to writable RAM
;pre: MAXBUF is defined
;post: INBUFF contains read characters
;post: C register contains byte count
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
READLN:	LXI	H, INBUFF	;Point to start of buffer
	MVI	C, 0		;C = byte counter
READL1:	CALL	CINNE		;Get console char, no echo
	CPI	BS		;Backspace?
	JZ	READL2		;Yes, handle it
	CALL	COUT		;Not BS, echo it
	CPI	CR
	RZ			;Got CR, done
	MOV	M, A		;Not CR, add to buffer
	INR	C		;Increment byte count
	INX	H		;Increment buffer pointer
	CPI	MAXBUF		;Full?
	JNZ	READL1		;No, get more
	RET

READL2:	DCR	C		;Decrement byte count
	JP	READL3		;Nonzero, modify buffer
	INR	C		;Buffer already empty
	JMP	READL1

READL3:	DCX	H		;Decrement buffer pointer
	CALL	COUT		;Erase character under cursor
	CALL	PRTSPC
	MVI	A, BS
	CALL	COUT
	JMP	READL1		;Read more chars

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SPLIT -- Break up a string
;
;This routine will split a string on predefined acceptable
;"whitespace," which may include punctuation.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
SPLIT:	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;TOKNIZ -- Tokenize a command string
;
;Token format:
;
;	Token type, one byte:
;		* 0x00 command 
;		* 0x01 byte parameter
;		* 0x02 double parameter
;		* 0x03 variable length data parameter
;
;	Token contents, 16-bit value
;
;	Command token points to command linked list entry
;	Byte parameter contains the byte parameter
;	Double parameter contains the double
;	Variable length data param points to the data
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
TOKNIZ:	
