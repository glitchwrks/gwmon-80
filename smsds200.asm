;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SMSDS100 -- GWMON-80 Small Monitor for SD Systems SBC-200
;
;This customization works on the SD Systems SBC-200, see 
;SMSDS100 for a version for the SBC-100.
;
;This customization uses the default SDMON configuration:
;
;	* ROM containing GWMON-80 at 0xE000
;	* 1K of scratchpad RAM at 0xFC00
;	* Serial port at 0x7C, 0x7D
;	* CTC for bitrate generation at 0x78
;
;The ROM may be any of the types supported by the SBC-200.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Hardware Equates
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
BRGPRT	equ	078H		;Port for CTC channel used as bitrate
DATPRT	equ	07CH		;SBC-200 data port
CTLPRT	equ	07DH		;SBC-200 control/status port
SBCCTL	equ	07FH		;SBC-200 board control port
STACK	equ	0FFFFH		;Stack at top of scratchpad

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SD Systems SBC-200 CTC Bitrate Table
;
;This table only supports bitrates that work with a /16
;divisor in the 8251 USART. 150 BPS and 300 BPS are possible
;with a /64 divisor. See the SBC-200 manual.
;
;Set SPEED equal to one of these values to select bitrate.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
B9600	equ	00DH
B4800	equ	01AH
B2400	equ	034H
B1200	equ	068H
B600	equ	0D0H

SPEED	equ	B9600		;Default bitrate is 9600 BPS

	ORG	0E000H

	INCLUDE	'vectors.inc'	;Standard GWMON-80 jump table

CSTART:	IN	SBCCTL		;Turn off autostart circuit

	MVI	A, 45H		;Set up CTC for bitrate generation
	OUT	BRGPRT
	MVI	A, SPEED
	OUT	BRGPRT

	INCLUDE	'sm.inc'	;The small monitor
	INCLUDE	'scmdstd.inc'	;SM standard commands
	INCLUDE	'scmdnull.inc'	;Command table terminator

	INCLUDE	'i8251a.inc'	;Generic Intel 8251 I/O

	END
