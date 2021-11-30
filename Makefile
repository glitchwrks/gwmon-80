RMFLAGS		= -f

ASM		= a85
RM		= rm

all: 8085r3 mits1

8085r3: sm8085r3

sm8085r3:
	$(ASM) sm8085r3.asm -o sm8085r3.hex -l sm8085r3.prn

mits1: smmits1

smmits1:
	$(ASM) smmits1.asm -o smmits1.hex -l smmits1.prn

clean:
	$(RM) $(RMFLAGS) *.hex *.prn

distclean: clean
