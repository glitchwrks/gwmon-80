;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SMCPM80 -- GWMON-80 Small Monitor for CP/M-80
;
;This customization uses CP/M console I/O. It is compatible
;with CP/M 1.4 through 3.0. It is intended primarily for
;testing GWMON-80 development.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Hardware Equates
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
STACK	equ	03FEH		;Start above monitor
CPMSTK	equ	STACK		;Store CP/M stack above our own

	ORG	0100H		;Load in standard TPA

	INCLUDE	'vectors.inc'	;Standard GWMON-80 jump table

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SAVSTK -- Save the CP/M stack
;
;The current stack pointer is saved at CPMSTK for later use
;by the 'C' command.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
SAVSTK:	LXI	H, 0		;Zero HL
	DAD	SP		;HL = CP/M stack pointer
	SHLD	CPMSTK		;Save for later

	INCLUDE	'sm.inc'	;The small monitor
	INCLUDE	'scmdstd.inc'	;SM standard commands

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SCMDCPM -- SM Quit Command under CP/M
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CPMCMD:	db	'c'
	dw	CPMQUT

	INCLUDE 'scmdnull.inc'	;Command table terminator

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;CPMQUIT -- Exit to CP/M
;
;This command restores the CP/M stack pointer from CPMSTK
;and returns through BDOS call. CPMSTK must be initialized
;by the customization.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CPMQUT:	LHLD	CPMSTK		;HL = CP/M stack pointer
	SPHL			;Restore CP/M stack
	RET			;Return to CP/M

	INCLUDE 'cpm80io.inc'	;CP/M-80 console I/O

	END
