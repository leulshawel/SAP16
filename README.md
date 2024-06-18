<b><h1>SAP 16</h1></b> This is an emluater for a simple as possible (SAP) CPU with dev tool-kit

got experience in CPU architecture and assembly? then just take a look at the ./test.asm example code<br>
you better on C? then take a llok at the CPU emulator source code<br>
you want the list of instructions and some dettails on their workings? then take a look at doc.txt

<b><h3>getting started</h3></b>

do the usual

```sh 
git clone https://github.com/leulshawel/core16
make cpu16
./cpu16  -d <memorydumpfile> -c <number of cores>
```
flags:
    -s save the context of cpu to file<br>
    -l load a context to continue execution<br>
    -d dump the memory only<br>
    -c set number of cores<

<b><h3>writting your own piece of code</h3></b>

1. write some code in the custom assembly languange
2. assemble your code using the assembler which will produce rom.bin
3. run ./cpu16
```sh
python3 assembler.py 
```

