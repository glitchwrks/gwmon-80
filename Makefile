RMFLAGS		= -f

ASM		= a85
RM		= rm

all: 8085r3 cpm80 cscc mits1 tdlsmb

8085r3: sm8085r3

sm8085r3:
	$(ASM) sm8085r3.asm -o sm8085r3.hex -l sm8085r3.prn

cpm80: smcpm80

smcpm80:
	$(ASM) smcpm80.asm -o smcpm80.hex -l smcpm80.prn

cscc: smcscc

smcscc:
	$(ASM) smcscc.asm -o smcscc.hex -l smcscc.prn

mits1: smmits1

smmits1:
	$(ASM) smmits1.asm -o smmits1.hex -l smmits1.prn

tdlsmb: smtdlsmb

smtdlsmb:
	$(ASM) smtdlsmb.asm -o smtdlsmb.hex -l smtdlsmb.prn

clean:
	$(RM) $(RMFLAGS) *.hex *.prn

distclean: clean
