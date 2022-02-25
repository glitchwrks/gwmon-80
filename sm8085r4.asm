;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SM8085R4 -- GWMON-80 Small Monitor for 8085 SBC rev 4
;
;This customization uses the console port on the Glitch
;Works 8085 SBC rev 4.
;
;Currently in prototype testing state, this version expects
;to run at 0x0000 on an external mixed memory board. This
;requires a modified development GAL.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Hardware Equates
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CTLPRT	equ	001H		;8085 SBC rev 4 console control port
DATPRT	equ	000H		;8085 SBC rev 4 console data port
STACK	equ	0E000H		;Stack below RAM-resident GWMON

	ORG	0000H

	INCLUDE	'vectors.inc'	;Standard GWMON-80 jump table

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;CSTART -- Customization-specific cold start routine
;
;Switches out ROM on 8085 SBC rev 3 and falls through to
;the monitor.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CSTART:	

; No ROM control for now, as we'll run off an external board.
;	MVI	A, 010H		;Turn off ROM at 0x0000
;	OUT	02H

	INCLUDE	'sm.inc'	;The small monitor
	INCLUDE	'scmdstd.inc'	;SM standard commnads
	INCLUDE	'scmdnull.inc'	;Command table terminator

	INCLUDE	'i8251.inc'	;Generic Intel 8251 USART

	END
