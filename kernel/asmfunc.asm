; asnfunc.asm
;
; System V AMD64 Calling Convention
; Registers: RDI, RSI, RDX, RCX, R8, R9

bits 64
section .text

; SYsten V ANS64 ABI の仕様により
; 二つの引数 addr と data はそれぞれ RDI, RSI レジスタに入る
global IoOut32 ; void IoOut32(uint16_t addr, uint32_t data);
IoOut32:
    mov dx, di    ; dx = addr
    mov eax, esi  ; eax = data
    out dx, eax
    ret

; RAX レジスタの値が戻り値になる
global IoIn32  ; uinbt32_t IoIn32(uint16_t addr);
IoIn32:
    mov dx, di    ; dx = addr
    in eax, dx
    ret
