#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <signal.h>


#define word_size16  //A single Word is 16 Bits or 2 Bytes
#define memory_addr_space 65536 //The max Address the cpu can address
#define ram_size memory_addr_space * 2 //Ram size in bytes
#define FP 13 //Index of Frame Pointer in the register array 
#define SP 14 //Index of Stack Pointer in the register array 
#define PC 15 //Index of Program Counter in the register array 
#define regNum 16 //Number of registers
#define ENTRY 65535  //Address of reset vector
#define INT_VECT 65534 //Address of Intrupt vector
#define CORENUM 3
#define RAMSIZE 2000





typedef __uint16_t word; 
typedef unsigned char int8;	


typedef struct{
    word ram[RAMSIZE];
    word rom[memory_addr_space - RAMSIZE];
}Memory;

typedef struct 
{
  //word* memory;
  word regs[regNum];
  __int8_t status; //XXXGLECZ
  __int8_t coreId;
  __int8_t sleep;
}Core;

typedef struct
{
  word memory[memory_addr_space];
  Core cores[CORENUM];
  __int8_t corenum;
}Cpu;

Cpu cpu;

void mem_dump(word start, word end){
  for (int i=start; i<=end; i++){
    printf("%04x: %04x  ", i, cpu.memory[i]);
    if (i%5 == 0)
      printf("\n");
  }
}

//save the context of the cores
void saveStateFile(char* path){
  word padding[regNum];
  FILE* stateFile = fopen(path, "ab");
  fwrite(cpu.memory, 2, memory_addr_space, stateFile);    
  fwrite(&cpu.corenum, 1, 1, stateFile);

  for (int i=0; i < cpu.corenum; i++){
    fwrite(cpu.cores[i].regs, 2, regNum, stateFile);
    fwrite(&cpu.cores[i].sleep, 1, 1, stateFile);
    fwrite(&cpu.cores[i].coreId, 1, 1, stateFile);    
    fwrite(&cpu.cores[i].status, 1, 1, stateFile);    
  } 
    fwrite(padding, 1, regNum, stateFile);    

}

//load a context
void loadStateFile(char* path){
    int index =  2*memory_addr_space;
    int size = memory_addr_space + regNum*cpu.corenum;
    word temp[size];
    char* regStart;
    
    FILE* stateFile = fopen(path, "rb");
    fread(temp, 1, 2*size+3*cpu.corenum+1, stateFile);
    memcpy(cpu.memory, temp, memory_addr_space*2);
    regStart = temp + memory_addr_space;
    memcpy(&cpu.corenum, regStart, 1);                //the number of cores in the state file -c can overwrite this
    regStart++;
    for (int i=0; i < cpu.corenum; i++){
      memcpy(cpu.cores[i].regs, regStart, regNum*2);  //load the registerd
      regStart += regNum * 2;
      memcpy(&cpu.cores[i].sleep, regStart, 1);       //the sleep status if a core
      regStart++;
      memcpy(&cpu.cores[i].coreId,  regStart, 1);     //coreId for that context
      regStart++;
      memcpy(&cpu.cores[i].status, regStart, 1);      //flags of a core
      regStart++;
    }
}

//write the contents of memory to the memory file
void dumpRamFile(char* path){
  FILE* ramFile = fopen(path, "wb");
  fwrite(cpu.memory, 2, memory_addr_space, ramFile);
}


