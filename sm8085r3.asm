;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SM8085R3 -- GWMON-80 Small Monitor for 8085 SBC rev 3
;
;This customization uses the console port on the Glitch
;Works 8085 SBC rev 3.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Hardware Equates
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CTLPRT	equ	001H		;8085 SBC rev 3 control port
DATPRT	equ	000H		;8085 SBC rev 3 data port
STACK	equ	0E000H		;Stack below RAM-resident GWMON

	ORG	0E000H

	INCLUDE	'vectors.inc'	;Standard GWMON-80 jump table

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;CSTART -- Customization-specific cold start routine
;
;Switches out ROM on 8085 SBC rev 3 and falls through to
;the monitor.
;
;The label for CSTART is defined in VECTORS.INC
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	MVI	A, 010H		;Turn off ROM at 0x0000
	OUT	02H

	INCLUDE	'sm.inc'	;The small monitor
	INCLUDE	'scmdstd.inc'	;SM standard commnads
	INCLUDE	'scmdnull.inc'	;Command table terminator

	INCLUDE	'i8251a.inc'	;Generic Motorola 6850 ACIA I/O

	END
