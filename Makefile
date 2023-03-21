RMFLAGS		= -f

ASM		= a85
RM		= rm

all: 8085r1 8085r3 certek2 cpm80 cscc imsai1 mits1 tdlsmb vgzcb

8085r1: sm8085r1

sm8085r1:
	$(ASM) sm8085r1.asm -o sm8085r1.hex -l sm8085r1.prn

8085r3: sm8085r3

sm8085r3:
	$(ASM) sm8085r3.asm -o sm8085r3.hex -l sm8085r3.prn

certek2: smcertk2

smcertk2:
	$(ASM) smcertk2.asm -o smcertk2.hex -l smcertk2.prn

cpm80: smcpm80

smcpm80:
	$(ASM) smcpm80.asm -o smcpm80.hex -l smcpm80.prn

cscc: smcscc

smcscc:
	$(ASM) smcscc.asm -o smcscc.hex -l smcscc.prn

imsai1: smimsai1

smimsai1:
	$(ASM) smimsai1.asm -o smimsai1.hex -l smimsai1.prn

mits1: smmits1

smmits1:
	$(ASM) smmits1.asm -o smmits1.hex -l smmits1.prn

tdlsmb: smtdlsmb

smtdlsmb:
	$(ASM) smtdlsmb.asm -o smtdlsmb.hex -l smtdlsmb.prn

vgzcb: smvgzcb

smvgzcb:
	$(ASM) smvgzcb.asm -o smvgzcb.hex -l smvgzcb.prn

clean:
	$(RM) $(RMFLAGS) *.hex *.prn

distclean: clean
