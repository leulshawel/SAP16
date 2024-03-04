.org #0
result:

result= #1

.org %8000
.start
movi r1 #1


main:
xor r0 r0
xchg r0 %0
cmp r0 r1
je $master
jmp $main

master:
str r1 $result
slp








.org %1000
.intvec
int_handler:
halt