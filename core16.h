#define word_size16  //A single Word is 16 Bits or 2 Bytes
#define memory_addr_space 65536 //The max Address the cpu can address
#define ram_size memory_addr_space * 2 //Ram size in bytes
#define FP 13 //Index of Frame Pointer in the register array 
#define SP 14 //Index of Stack Pointer in the register array 
#define PC 15 //Index of Program Counter in the register array 
#define regNum 16 //Number of registers
#define ENTRY 65535  //Address of reset vector
#define INT_VECT 65534 //Address of Intrupt vector
#define STACK 256
typedef __uint16_t word; 
typedef unsigned char int8;	


