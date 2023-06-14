
.align   4

.global enable_sse
enable_sse:
    movl $0x1, %eax
    cpuid
    test $1<<25, %edx
    jz noSSE
    test $1<<26, %edx
    jz noSSE
    fninit

    movl %cr0, %eax
    andl $0xFFFB, %eax		#clear coprocessor emulation CR0.EM
    orl $0x22, %eax			#set coprocessor monitoring  CR0.MP
    movl %eax, %cr0
    movl %cr4, %eax
    orl $0x600, %eax		#set CR4.OSFXSR and CR4.OSXMMEXCPT at the same time
    movl %eax, %cr4
noSSE:
    ret
