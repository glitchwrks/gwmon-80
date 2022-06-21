;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SMVGZCB -- GWMON-80 Small Monitor for Vector Graphic ZCB
;
;This customization uses the default serial port
;configuration on the ZCB as the console. It expects default
;memory configuration:
;
;	* ROM containing GWMON-80 at 0xE000
;	* 1K of scratchpad RAM at 0xFC00
;
;The ROM may be any of the types supported by the ZCB.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Hardware Equates
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CTLPRT	equ	005H		;Default ZCB control port
DATPRT	equ	004H		;Default ZCB data port
STACK	equ	0FFFFH		;Stack at top of scratchpad

	ORG	0E000H

	INCLUDE	'vectors.inc'	;Standard GWMON-80 jump table

	INCLUDE	'sm.inc'	;The small monitor
	INCLUDE	'scmdstd.inc'	;SM standard commnads
	INCLUDE	'scmdnull.inc'	;Command table terminator

	INCLUDE	'i8251a.inc'	;Generic Intel 8251 I/O

	END
