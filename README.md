<b><h1>SAP 16</h1></b> This is an emluater for a simple as possible (SAP) 16bit CPU with dev tool-kit

<b><h3>getting started</h3></b>

do the usual

```sh 
git clone https://github.com/leulshawel/core16
make cpu16
./cpu16  -d <memorydumpfile> -c <number of cores>
```
flags:<br>
    -s save the context of cpu to file<br>
    -l load a context to continue execution<br>
    -d dump the memory only<br>
    -c set number of cores

<b><h3>writting your own piece of code</h3></b>

1. write some code in the custom assembly languange
2. assemble your code using the assembler which will produce rom.bin
```sh
python3 assembler.py <your-asm-file-path>
```
3. run ./cpu16


