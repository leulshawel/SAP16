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
  word* memory;
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


void saveStateFile(char* path){
  
  FILE* stateFile = fopen(path, "ab");
  fwrite(cpu.memory, 2, memory_addr_space, stateFile);
  for (int i=0; i < cpu.corenum; i++){
    fwrite(cpu.cores[i].regs, 2, regNum, stateFile);
    fwrite(&cpu.cores[i].sleep, 1, 1, stateFile);
  } 
}

//this shit is not working :(
void loadStateFile(char* path){
    int index =  2*memory_addr_space;
    int size = memory_addr_space+32*(cpu.corenum)+cpu.corenum;
    word temp[size];
    
    FILE* stateFile = fopen(path, "rb");
    fread(temp, 2, size, stateFile);
    memcpy(cpu.memory, temp, memory_addr_space*2);

    for (int i=0; i < cpu.corenum; i++){
      memcpy(cpu.cores[i].regs, &temp[index], 32);
      memcpy(&cpu.cores[i].sleep, &temp[index+32], 1);
      index =+33;
    }
}

void dumpRamFile(char* path){
  FILE* ramFile = fopen(path, "wb");
  fwrite(cpu.memory, 2, memory_addr_space, ramFile);
}


