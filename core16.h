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
  __int8_t status; //XXXXLNCZ
  __int8_t coreId;
  __int8_t sleep;
}Core;

typedef struct
{
  word memory[memory_addr_space];
  Core cores[CORENUM];
}Cpu;




