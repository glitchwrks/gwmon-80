;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;8085SIO1 -- Console I/O Drivers for 8085 Built-In Serial
;
;Use the SID and SOD pins on the Intel 8085 for bit-bang
;serial I/O. This driver uses fixed bitrate constants for 
;delay routines used to pace serial transmit/receive.
;
;Derived from Intel Application Note AP-29, August 1977.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

BITTIME	equ	0112H		;9600 BPS with 6.144 MHz crystal
HALFBIT	equ	0109H		;Half-bit time

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SETUP -- Prepare the system for running the monitor
;
;Nothing to do here.
;
;pre: none
;post: console ready for I/O
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
IOSET:	RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;CINNE -- Get a char from the console, no echo
;
;pre: BITTIME, HALFBIT initialized
;post: received char is in A register
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CINNE:	PUSH	B		;Preserve registers
	PUSH	H
	DI			;Must disable interrupts
	MVI	B, 9		;Receive bits counter

CI1:	RIM			;Check for start bit
	ORA	A
	JM	CI1		;No start bit, wait

	LXI	H, HALFBIT	;Delay one half-bit time, this puts
CI2:	DCR	L		;us in the middle of the start bit
	JNZ	CI2
	DCR	H
	JNZ	CI2

CI3:	LXI	H, BITTIME	;Delay one bit-time, since we are shifted
CI4:	DCR	L		;a half-bit time through the start bit, all
	JNZ	CI4		;this will keep us in the middle of bits for
	DCR	H		;reliable sampling.
	JNZ	CI4

	RIM			;Read the SID line
	RAL			;CY = data bit
	DCR	B		;Determine if this is a stop bit
	JZ	CI5		;Stop bit, done with char receive

	MOV	A, C		;Not done, rotate a bit into C
	RAR
	MOV	C, A
	NOP			;Equalize COUT and CINNE loop times
	JMP	CI3		;Get more bits

CI5:	POP	H		;Restore HL
	MOV	A, C		;A = received character
	POP	B		;Restore BC
	EI
	CPI	CANCEL		;Check for CANCEL character
	JZ	WSTART		;Yes, warm start monitor
	RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;COUT -- Output a character to the console
;
;This routine *must* preserve the contents of the A register
;or CIN will not function properly.
;
;pre: A register contains char to be printed
;pre: BITTIME is initialized
;post: character is printed to the console
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
COUT:	PUSH	PSW		;Save char to print on stack
	PUSH	B		;Preserve registers
	PUSH	H
	DI			;Must disable interrupts
	MOV	C, A		;C = character to output
	XRA	A		;Clear CY
	MVI	B, 10H		;Bit count for entire serial transaction

CO1:	MVI	A, 80H		;Set what will become SOD enable bit
	RAR			;Move CY into SOD data bit
	SIM			;Output it on SOD

	LXI	H, BITTIME	;Delay one bit time
CO2:	DCR	L
	JNZ	CO2
	DCR	H
	JNZ	CO2

	STC			;Set CY, will become stop bit	
	MOV	A, C		
	RAR			;Get a bit from the byte to output into CY
	MOV	C, A
	DCR	B		;Decrement bit count
	JNZ	CO1		;Not done, send more bits

	POP	H		;Done, restore registers
	POP	B
	POP	PSW
	EI
	RET
