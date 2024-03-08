run: cpu16 rom.bin
	-rm statefile.bin
	./cpu16 

core16: cpu16.c cpu16.h
	gcc -o core16 core16.c

rom.bin: test.asm
	python3 assembler.py test.asm

clear:
	-rm cpu16 rom.bin statefile.bin 