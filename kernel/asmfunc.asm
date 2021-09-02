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

global GetCS ; uint16_t GetCS(void);
GetCS:
    xor eax, eax ; also clears upper 32bits of rax
    mov ax, cs
    ret

global LoadIDT ; void LoadIDT(uint16_t limit, uint64_t offset);
LoadIDT:
    push rbp
    mov rbp, rsp
    sub rsp, 10
    mov [rsp], di ; limit
    mov [rsp + 2], rsi ; offset
    lidt [rsp]
    mov rsp, rbp
    pop rbp
    ret
