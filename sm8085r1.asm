;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SM8085R1 -- GWMON-80 Small Monitor for 8085 SBC rev 1
;
;This customization uses the 8085's built-in serial I/O and
;is configured for the Glitch Works 8085 SBC rev 1 memory
;map.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Hardware Equates
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
STACK	equ	0000H		;Stack at top of RAM, modified R1 board

	ORG	0000H		;Reset jump to monitor, leave room for
	JMP	CSTART		;interrupt vectors.

	ORG	0100H		;Actual start of monitor

	INCLUDE	'vectors.inc'	;Standard GWMON-80 jump table

	INCLUDE	'sm.inc'	;The small monitor
	INCLUDE	'scmdstd.inc'	;SM standard commands
	INCLUDE	'scmdnull.inc'	;Command table terminator

	INCLUDE	'8085sio1.inc'	;Intel 8085 SID/SOD driver

	END
