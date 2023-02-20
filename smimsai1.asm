;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SMIMSAI1 -- GWMON-80 Small Monitor for IMSAI SIO2
;
;This customization uses an IMSAI SIO2 configured for
;standard options:
;
;	* I/O mapped ports
;	* Base address of 0x00
;	* Polled I/O, no interrupts
;
;Default configuration assumed here expects GWMON-80 to be
;ROMed at 0xF000 and RAM available immediately below that.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Hardware Equates
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CTLPRT	equ	003H		;Default SIO2 Channel A control port
DATPRT	equ	002H		;Default SIO2 Channel A data port
STACK	equ	0F000H		;Stack below ROM

	ORG	0F000H

	INCLUDE	'vectors.inc'	;Standard GWMON-80 jump table

	INCLUDE	'sm.inc'	;The small monitor
	INCLUDE	'scmdstd.inc'	;SM standard commands
	INCLUDE	'scmdnull.inc'	;Command table terminator

	INCLUDE	'i8251.inc'	;Generic Intel 8251 I/O

	END
