<b><h1>SAP 16</h1></b> This is an emluater for a simple as possible (SAP) CPU with dev tool-kit

got experience in CPU architecture and assembly? then just take a look at the ./test.asm example code
you better on C? then take a llok at the CPU emulator source code
you want the list of instructions and some dettails on their workings? then take a look at doc.txt

<b><h4>getting started</h4></b>



```sh 
git clone https://github.com/leulshawel/core16
make cpu16
./cpu16  -d <memorydumpfile> -c <number of cores>
```
flags:
    -s save the context of cpu to file
    -l load a context to continue execution
    -d dump the memory only
    -c set number of cores

<b><h4>writting your own piece of code</h4></b>

1. write some code in the custom assembly languange
2. assemble your code using the assembler 
```sh
python3 assembler.py 
```

