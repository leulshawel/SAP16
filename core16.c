#include "core16.h"

//read memory file in to the ram
void read_memory(char* path){
  FILE* ram;
  ram = fopen(path, "rb");
  fread(cpu.memory, 2, memory_addr_space, ram);
}


//print the current state of the cores
void dump(__int8_t corenum){
  Core* core;
  //loop though all the cores printing the registers
  for (int i=0; i < corenum; i++){
    core = &cpu.cores[i];
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
    printf("inst: %04x   LNCZ: %x\n", cpu.memory[core->regs[PC]], core->status); //print the current instruction and status flags
  }
}



int main(int argc, char** argv){
  int ch; //for arg parssing
  Core* core; //what core are we on

  //is a spacific flag set
  bool lflag = false; //load state flag
  bool dflag = false; //dum memory flag
  bool sflag = false; //save state flag

  //This hold the argumnts of the options
  char* statefile = NULL; //save and load state file path
  char* memfile = NULL;   //dump memory file path

  bool clock = true;      //the clock of the cpu 
  cpu.corenum = CORENUM; //default number of cores (can be changed using -c)

  word inst, opcode;      //instruction and opcode
  __int16_t temp;         //hold intermediate values during instruction excution 
  word args = 0;          //the arguments of an insruction
  printf("\n");


  //ctrl + c handler
  void kill_hundler(){
    clock = false;
  }
  signal(SIGINT, kill_hundler); 

  //parse arguments passed
  while((ch = getopt(argc, argv, "d:l:s:c:")) != -1){
    switch (ch)
    {
    case 'l':
      lflag = true;
      statefile = optarg;
      break;
    case 's':
      sflag = true;
      statefile = optarg;
      break;
    case 'd':
      dflag = true;
      memfile = optarg;
    case 'c':
      cpu.corenum = (__int8_t) atoi(optarg);
      break;
    }
  }



  //read rom.bin file and set up the cores if we are not loading a state
  if (!lflag){
    read_memory("rom.bin"); 
    for (int i=0; i < cpu.corenum; i++){
      core = &cpu.cores[i];
      core->regs[PC] = cpu.memory[ENTRY];  //move the reset vector in to the program couter
      core->coreId = i;                    //assign all cores an id (used by id instruction)
      core->sleep = 0;                     //this halts the core if set to 1 (only that core)
    }
  }else loadStateFile(statefile);          //load a state from a file if -l option provided

  //fetch -> decode -> excute cycle
  while (clock){ 
    //loop though all the cores excuting one instruction per core
    for (int i=0; i < cpu.corenum; i++){
      core = &cpu.cores[i];
      if (core->sleep) continue; //continue if that core is sleep

      //fetch
      inst = cpu.memory[core->regs[PC]];
      core->regs[PC]++;

      //decode
      opcode = (inst & 0xff);         //opcode
      int8 r1 = (inst & 0x0f00) >> 8; //dest reg
      int8 r2 = inst >> 12;           //src reg
      
      //Excute  //the biggest switch statement i ever wrote
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
            if ( temp < 0) core->status |= 8;  //less
            else if (temp == 0) core->status |= 4; //equal
            else core->status |= 16; //greater
            temp = 0x1;
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
        case 0x15: //INT
            cpu.memory[core->regs[SP]] = core->regs[PC];
            core->regs[SP]--;
            core->regs[PC] = cpu.memory[INT_VECT];
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
        case 0x1f: //JE
            if ((core->status & 0x4) != 0)
              core->regs[PC] = cpu.memory[core->regs[PC]];
            else
              core->regs[PC]++;
              break;
        case 0x20: //JL
            if ((core->status & 0x8) != 0)
              core->regs[PC] = cpu.memory[core->regs[PC]];
            else
              core->regs[PC]++;
              break;
        case 0x21: //JG
            if ((core->status & 0x10) != 0)
              core->regs[PC] = cpu.memory[core->regs[PC]];
            else
              core->regs[PC]++;
              break;
        case 0x1c:  //NOT
            core->regs[r1] = !core->regs[r1];
            break;
        case 0x1d:  //ID
            core->regs[r1] = core->coreId;
            break;
        case 0x1e:  //SLP
            core->sleep = 1;
            break;
      }
      //Cheking destination register and setting correspondig flag bits
      if(opcode > 0x4 && opcode < 0xf){
        if (temp == 0) core->status = 1;
        else if (temp > 65535) core->status = 2; //
      }
      
      if (!clock) break;
    }
  }
  //save memory or state to file depending on options passed 
  //before dumping the cpu and exiting the program
  if (dflag)
    ramFileDump(memfile);
  if (sflag)
    saveStateFile(statefile);
  dump(cpu.corenum);
  return 0;
}
