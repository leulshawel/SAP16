.org %fffe
sys_handler_addr:
.org %ff00
int_vec_table:



int_vec_table= $sys_write
sys_handler_addr= $sys_call



.org %1000
sys_write:
jmp $halt

sys_read:
jmp $halt

sys_exit:
jmp $halt




.org %0
.start
os:
movi r1 #0
sys

.org %8000
sys_call:
movi r0 $int_vec_table
add r0 r1
ldrr r0
callr r0









halt:
halt
