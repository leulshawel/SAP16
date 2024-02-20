#include <stdio.h>
#include <stdlib.h>
#include "core16.h"
#include <unistd.h>
#include <stdbool.h>
#include <string.h>





Cpu cpu;

void read_memory(char* path){
  
  FILE* ram;
  ram = fopen(path, "rb");
  
  fread(cpu.memory, 2, memory_addr_space, ram);
}


void dump(Core* core){
  printf("core:  %d\n", core->coreId);
  for (int i=0; i < regNum; i++){
    if (i==PC)
      printf("%s", "PC ");
    else if (i==SP)
      printf("%s", "SP ");
    else if (i==FP)
	printf("%s", "FP ");
    else
      printf("%s%d", "R", i);
    

    if ((i) < 10)
	    printf(" ");
    printf(" : ");
    printf("%04x  ", core->regs[i]);
    if ((i+1)%4 == 0)
	    printf("\n");
    }
  printf("inst: %04x\n", cpu.memory[core->regs[PC]]);
}

void mem_dump(word start, word end){
  for (int i=start; i<=end; i++){
    printf("%04x: %04x  ", i, cpu.memory[i]);
    if (i%5 == 0)
      printf("\n");
  }
}

void saveStateFile(char* path, Core* core){
  FILE* stateFile = fopen(path, "ab");
  fwrite(cpu.memory, 2, memory_addr_space, stateFile);
  for (int i=0; i < CORENUM; i++)
    fwrite(cpu.cores[i].regs, 2, regNum, stateFile);

}

void loadStateFile(char* path){
    word temp[memory_addr_space+16];
    FILE* stateFile = fopen(path, "rb");
    fread(temp, 2, memory_addr_space+(16*CORENUM), stateFile);
    memcpy(cpu.memory, temp, memory_addr_space*2);
    
    for (int i=0; i < CORENUM; i++)
      memcpy(cpu.cores[i].regs, &temp[memory_addr_space+(i*16)], 32);
  
}

void ramFileDump(char* path){
  FILE* ramFile = fopen(path, "wb");
  fwrite(cpu.memory, 2, memory_addr_space, ramFile);
}


int main(int argc, char** argv){

  Core* core;
  bool loadState = false;

  bool clock = true;
  word inst, opcode;
  __int16_t temp;
  word args = 0;  printf("\n"); //0000LNCZR1R1R1R1XXXX

  
  if (!loadState){
    read_memory("rom.bin"); //read main memory (ram) file
    for (int i=0; i < CORENUM; i++){
    core = &cpu.cores[i];
    core->regs[PC] = cpu.memory[ENTRY]; //program entry point
    core->coreId = i;
    core->sleep = 0;
    }
  }else loadStateFile("state.bin");

  while (clock){ 
    for (int i=0; i < CORENUM; i++){
      core = &cpu.cores[i];
      if (core->sleep) continue;
      dump(core);

      //fetch
      inst = cpu.memory[core->regs[PC]];
      core->regs[PC]++;

      //decode
      opcode = (inst & 0xff); //opcode
      int8 r1 = (inst & 0x0f00) >> 8; //des reg
      int8 r2 = inst >> 12; //src reg
      
      //Excute
      switch(opcode){
        case 0x0:
            break;
        case 0x1: //LOAD
            core->regs[r1] = cpu.memory[cpu.memory[core->regs[PC]]];
            core->regs[PC]++;
            break;
        case 0x2: //SOTRE
            cpu.memory[cpu.memory[core->regs[PC]]]= core->regs[r1];
            core->regs[PC]++;
            break;
        case 0x3: //MOVE
            core->regs[r2] = core->regs[r1];
            break;
        case 0x4: //MOVE IMM
            core->regs[r1] = cpu.memory[core->regs[PC]];
            core->regs[PC]++;
            break;
        case 0x16: //LOAD MEM[REG[r2]]
            core->regs[r1] = cpu.memory[core->regs[r2]];
            break;
        case 0x18: //STORE MEM[REG[r2]]
            cpu.memory[core->regs[r2]] = core->regs[r1];
            break;
        case 0x5: //ADD
            temp = core->regs[r1] + core->regs[r2];
            core->regs[r1] = temp;
            break;
        case 0x6: //SUB
            temp = core->regs[r1] - core->regs[r2];
            core->regs[r1] = temp;
        case 0x7: //AND
            temp = core->regs[r1] &core->regs[r2];
            core->regs[r1] = temp;
            break;
        case 0x8: //OR
            temp = core->regs[r1] | core->regs[r2];
            core->regs[r1] = temp;
            break;
        case 0x9: //XOR
            temp = core->regs[r1] ^ core->regs[r2];
            core->regs[r1] = temp;
            break;
        case 0xa: //SHIFT LEFT
            temp = core->regs[r1] << core->regs[r2];
            core->regs[r1] = temp;
            break; //SGIFT RIGHT
        case 0xb:
            temp = core->regs[r1] >> core->regs[r2];
            core->regs[r1] = temp;          break;
        case 0xc: //INC
            core->regs[r1]++;
            temp = core->regs[r1];
            break;
        case 0xd: //DEC
            core->regs[r1]--;
            temp = core->regs[r1];
          break;
        case 0xe: //CMP
            temp = core->regs[r1] - core->regs[r2];
            if ( temp < 0) core->status = 8;
          break;
        case 0x10: //SPS
            core->regs[SP] = cpu.memory[core->regs[PC]];
            core->regs[FP] = core->regs[SP];
            core->regs[PC]++;
            break;
        case 0x11:  //PUSH
            cpu.memory[core->regs[SP]] = core->regs[r1];
            core->regs[SP]--;
            break;
        case 0x12: //POP
            core->regs[SP]++;
            core->regs[r1] = cpu.memory[core->regs[SP]];
            break;
        case 0xf: //HALT
            clock = false;
            break;
        case 0x13: //CALL
            cpu.memory[core->regs[SP]] = core->regs[PC]+1;
            core->regs[SP]--;
            cpu.memory[core->regs[SP]] = core->regs[FP];
            core->regs[SP]--;
            core->regs[FP] = core->regs[SP];
            core->regs[PC] = cpu.memory[core->regs[PC]];
            break;
        case 0x14: //RET
            core->regs[SP] = core->regs[FP]+2;
            core->regs[PC] = cpu.memory[core->regs[SP]];
            core->regs[FP] = cpu.memory[core->regs[SP]-1];
            break;
        case 0x15: //SYS
            int index = r1 | (r2 << 4);
            cpu.memory[core->regs[SP]] = core->regs[PC];
            core->regs[SP]--;
            core->regs[PC] = cpu.memory[INT_VECT]+index;
            break;
        case 0x17: //CALL MEM[REG[r1]]
            cpu.memory[core->regs[SP]] = core->regs[PC];
            core->regs[SP]--;
            cpu.memory[core->regs[SP]] = core->regs[FP];
            core->regs[SP]--;
            core->regs[FP] = core->regs[SP];
            core->regs[PC] = core->regs[r1];
            break;
        case 0x19: //JMP
            core->regs[PC] = cpu.memory[core->regs[PC]];
            break;
        case 0x1a: //JIZ
            if (core->status & 0x1)
              core->regs[PC] = cpu.memory[core->regs[PC]];
            else
              core->regs[PC]++;
            break;
        case 0x1b: //JOC
            if ((core->status & 0x2) >> 1)
              core->regs[PC] = cpu.memory[core->regs[PC]];
            else
              core->regs[PC]++;
              break;
        case 0x1c:
            core->regs[r1] = !core->regs[r1];
            break;
        case 0x1d:
            core->regs[r1] = core->coreId;
            break;
        case 0x1e:
            core->sleep = 1;
            break;
      }
      //Cheking destination register and setting correspondig flag bits
      if(opcode > 0x4 && opcode < 0xf){
        if (temp == 0) core->status = 1;
        else if (temp < 0) core->status = 4;
        else if (temp > 65535) core->status = 4;
      }
      printf("LNCZ: %x\n\n\n", core->status); //Print core status bits
      if (!clock) break;
    }
  }

  return 0;
}
