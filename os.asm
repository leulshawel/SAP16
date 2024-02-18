!the cpu loads the value here to the PC when a sys call happens
.org %fffe
sys_handler_addr:
int_vec_table= $sys_write


!build a table of different sys call hanler addresses
.org %ff00
int_vec_table:
sys_handler_addr= $sys_call


!handle write sys call here
.org %1000
sys_write:
ret


!handle read sys call here
sys_read:
ret

!handle exit sys call here
sys_exit:
ret



!Entry point of custom os
.org %0
.start
os:
id r0
movi r1 #256
movi r2 #1
movi r4 #0
add r0 r4
set_stack:
jiz $start
add r1 r1
sub r0 r2
jmp $set_stack
start:
mov r1 sp
jmp $sleep


!Main sys call handler routin
!looks at register values to figure sys call type
.org %8000
sys_call:
movi r0 $int_vec_table
add r0 r1
ldrr r0
callr r0
pop pc









halt:
halt

sleep:
slp