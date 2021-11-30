;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;VECTORS -- Jump Table Vectors
;
;These vectors are consistent across all versions of
;GWMON-80 >= 0.9.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
JMPTAB:	JMP	CSTART		;Cold start, initializes hardware
	JMP	WSTART		;Warm start
	JMP	COUT		;Output A register to console
	JMP	CIN		;Input A register from console, waits
CSTART:	
