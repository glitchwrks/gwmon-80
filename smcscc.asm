;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SMCSCC -- GWMON-80 Small Monitor for Cromemco SCC
;
;This customization uses the SCC's TMS 5501 for serial
;console, and the SCC's 1K of RAM for stack and storage. It
;expects to run from ROM at 0x0000.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Hardware Equates
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
TMSBASE	equ	00H		;TMS 5501 default addressing
STACK	equ	02400H		;Top of SCC onboard RAM

	ORG	0000H

	INCLUDE	'vectors.inc'	;Standard GWMON-80 jump table

	INCLUDE	'sm.inc'	;The small monitor
	INCLUDE	'scmdstd.inc'	;SM standard commands
	INCLUDE 'scmdnull.inc'	;Command table terminator

	INCLUDE 'tms5501.inc'	;Generic TI TMS 5501 I/O

SPEED	set	B9600		;Set the speed here, must be after
				;the TMS5501 INCLUDE

	END
