;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;XCMDNULL -- NULL Command
;
;This should be the last command list included in a XM
;customization. It provides consistent termination to the
;command list and allows potential expansion.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
NULCMD:	dw	TOPMEM - 16
