.org %8000
.start
main:
movi r1 #3
id r0
cmp r0 r1
je $write
slp
write:
str r0 %0
halt






.org %1000
.intvec
int_handler:
halt