.org #0
key:
key= #1


.org #100
counter:
counter= #1


.org %8000
.start
movi r1 #1


main:
xor r0 r0
xchg r0 $key
cmp r0 r1
je $master
jmp $main


master:
!get my counter address
ldr r3 $counter
id r0
strr r0 r3
!increment the counter address for the next core
inc r3
mov r3 r4
inc r4
str r4 $counter

!let the next core run
movi r4 #1
str r4 $key

!do my count


count_loop:
ldrr r0 r3
inc r0
strr r0 r3
jmp $count_loop









.org %1000
.intvec
int_handler:
halt