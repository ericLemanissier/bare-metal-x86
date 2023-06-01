.global _asm_schedule, _asm_timer, _asm_int_kbd, _asm_syscalls, _asm_exc_GP, _asm_exc_PF
.align   4

_asm_schedule:
    pushal
    cld
	call isr_schedule_int
    popal
	iret

_asm_timer:
    pushal
    cld
	call isr_timer_int
    popal
	iret


_asm_int_kbd:
    pushal
    cld
	call isr_kbd_int
    popal
	iret

_asm_syscalls:
    pushal
	call do_syscalls
    popal
	iret


_asm_exc_GP:
    pushal
	call isr_GP_exc
    popal
	# add esp,4
	iret

_asm_exc_PF:
    pushal
	call isr_PF_exc
    popal
	# add esp,4
	iret
