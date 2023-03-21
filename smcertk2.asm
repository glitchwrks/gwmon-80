;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SMSBC852 -- GWMON-80 Small Monitor for Certek SBC85-2
;
;This customization uses the 8085's built-in serial I/O and
;the RAM present in the Certek SBC85-2 8156 RIOT. It expects
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Hardware Equates
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
STACK	equ	1100H		;Stack at top of 8156 RAM

	ORG	0000H		;Reset jump to monitor, leave room for
	JMP	CSTART		;interrupt vectors.

	ORG	0100H		;Actual start of monitor

	INCLUDE	'vectors.inc'	;Standard GWMON-80 jump table

	INCLUDE	'sm.inc'	;The small monitor
	INCLUDE	'scmdstd.inc'	;SM standard commands
	INCLUDE	'scmdnull.inc'	;Command table terminator

	INCLUDE	'8085sio1.inc'	;Intel 8085 SID/SOD driver

	END
