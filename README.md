This is an emuater for a custom 16 bit arch SAP CPU with dev tool-kit

usage: ./core -sc [statefile] -d [memorydumpfile] 

Here you will find 
	
documentantion of the how this whole thing 'works' [DOC.txt],
the source code for the emulator [Core16.c],
a linux excutable binary of the core emulator [core],
an assembler for the custom assembly "language" written in python [assembler.py]

Not done working on options (arguments)
-s [statefile] save the state of the cpu to a statefile
-d dump the memory of cpu
-c load a state from statefile to continue running from that state
