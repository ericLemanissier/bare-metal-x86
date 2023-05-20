[ORG 0x7c00]      ; add to offsets
   jmp start

   %include "print.inc"

start:   xor ax, ax   ; make it zero
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
