AFLAGS		= -t none
LFLAGS		= -t none
RMFLAGS		= -f

ASM		= a85
RM		= rm

all: smmits1

smmits1:
	$(ASM) smmits1.asm -o smmits1.hex -l smmits1.prn

clean:
	$(RM) $(RMFLAGS) *.hex *.prn

distclean: clean
