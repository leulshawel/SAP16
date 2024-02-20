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
sys r1 r0


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