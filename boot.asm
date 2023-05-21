[ORG 0x7c00]      ; add to offsets
   jmp start

   %include "print.inc"

start:   xor ax, ax   ; make it zero
   mov ds, ax   ; DS=0
   mov ss, ax   ; stack starts at 0
   mov sp, 0x9c00   ; 2000h past code start

   cli      ; no interrupt
   push ds      ; save real mode

   lgdt [gdtinfo]   ; load gdt register

   mov  eax, cr0   ; switch to pmode by
   or al,1         ; set pmode bit
   mov  cr0, eax

   mov  bx, 0x08   ; select descriptor 1
   mov  ds, bx   ; 8h = 1000b

   and al,0xFE     ; back to realmode
   mov  cr0, eax   ; by toggling bit again

   pop ds      ; get back old segment
   sti

   mov bx, 0x0f01   ; attrib/char of smiley
   mov eax, 0x0b8000 ; note 32 bit offset
   mov word [ds:eax], bx
   mov ds, ax   ; DS=0
   mov ss, ax   ; stack starts at 0
   mov sp, 0x9c00   ; 200h past code start

   mov ax, 0xb800   ; text video memory
   mov es, ax

   cli      ;no interruptions
   mov bx, 0x09   ;hardware interrupt #
   shl bx, 2   ;multiply by 4
   xor ax, ax
   mov gs, ax   ;start of memory
   mov [gs:bx], word keyhandler
   mov [gs:bx+2], ds ; segment
   sti

   jmp $      ; loop forever

keyhandler:
   in al, 0x60   ; get key data
   mov bl, al   ; save it
   mov byte [port60], al

   in al, 0x61   ; keybrd control
   mov ah, al
   or al, 0x80   ; disable bit 7
   out 0x61, al   ; send it back
   xchg ah, al   ; get original
   out 0x61, al   ; send that back

   mov al, 0x20   ; End of Interrupt
   out 0x20, al   ;

   and bl, 0x80   ; key released
   jnz done   ; don't repeat

   mov ax, [port60]
   mov  word [reg16], ax
   call printreg16

done:
   iret

port60   dw 0

   times 510-($-$$) db 0  ; fill sector w/ 0's
   dw 0xAA55        ; req'd by some BIOSes
;==========================================
