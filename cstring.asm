    .code
; extern "C" char* StrCpy(char* dst, const char* src);
StrCpy proc frame
    push rsi
    .pushreg rsi
    .endprolog

    mov rsi, rdx
    mov rdx, rcx

@@:
    lodsb                    ; al = src[i++]
    mov byte ptr [rcx], al   
    inc rcx
    test al, al             
    jnz @B
    
    mov rax, rdx

    .beginepilog
    pop rsi
    ret
StrCpy endp

; extern "C" char* StrNCpy(char* dst, const char* src, size_t count);
StrNCpy proc frame
    push rsi
    .pushreg rsi
    .endprolog

    mov rsi, rdx
    mov rdx, rcx

@@:
    test r8, r8
    jz @F
    lodsb                   ; al = *rsi++
    mov byte ptr [rcx], al
    inc rcx
    dec r8
    test al, al
    jnz @B

@@:
    mov rax, rdx

    .beginepilog
    pop rsi
    ret
StrNCpy endp

; extern "C" char* StrCat(char* dst, const char* src);
; extern "C" char* StrNCat(char* dst, const char* src, size_t count);

; extern "C" size_t StrLen(const char* str);
StrLen proc frame
    push rdi
    .pushreg rdi
    .endprolog

    mov rdi, rcx
    mov rcx, -1
    xor al, al
    repne scasb
    not rcx
    dec rcx
    mov rax, rcx

    .beginepilog
    pop rdi
    ret
Strlen endp

; extern "C" int StrCmp(const char* lhs, const char* rhs);
; extern "C" int StrNCmp(const char* lhs, const char* rhs, size_t count);

; extern "C" const char* StrChr(const char* str, int ch);

; extern "C" const char* StrRChr(const char* str, int ch);

; extern "C" size_t StrSpn(const char* dst, const char* src);

; extern "C" size_t StrCSpn(const char* dst, const char* src);

; extern "C" const char* StrPbrk(const char* dst, const char* breakset);

; extern "C" const char* StrStr(const char* haystack, const char* needle);

; extern "C" char* StrTok(char* str, const char* delim);

; extern "C" const void* MemChr(const void* ptr, int ch, size_t count);

; extern "C" int MemCmp(const void* lhs, const void* rhs, size_t count);

; extern "C" void* MemSet(void* dst, int ch, size_t count)
MemSet proc frame
    push rdi
    .pushreg rdi
    .endprolog

    mov r10, rcx     ; r10 = dst
    xor eax, eax     
    mov al, dl
    test al, al      
    jnz Set1

Zero8:
    mov rcx, r8    
    mov r9, 0FFFFFFFFFFFFFFF8h ; ~(0x8)
    and rcx, r9
    jz Zero4
    sub r8, rcx
    shr rcx, 3                 ; rcx /= 8
    rep stosq    
    jmp Set1

Zero4:
    mov rcx, r8    
    mov r9, 0FFFFFFFFFFFFFFF4h ; ~(0x4)
    and rcx, r9
    jz Zero2
    sub r8, rcx
    shr rcx, 2                 ; rcx /= 4
    rep stosd   
    jmp Set1

Zero2:
    mov rcx, r8    
    mov r9, 0FFFFFFFFFFFFFFF2h ; ~(0x4)
    and rcx, r9
    jz Set1
    sub r8, rcx
    shr rcx, 1                 ; rcx /= 2
    rep stosw  
    jmp Set1 

Set1:
    mov rcx, r8
    rep stosb 

    mov rax, r10

    .beginepilog
    pop rdi
    ret
MemSet endp

; extern "C" void* MemCpy(void* dst, const void* src, size_t count);
MemCpy proc frame
    push rsi
    .pushreg rsi
    push rdi
    .pushreg rdi
    .endprolog

    mov rax, rcx    ; ret = dst
    mov rdi, rcx    ; rdi = dst
    mov rsi, rdx    ; rsi = src

Copy8:
    mov rcx, r8    
    mov r9, 0FFFFFFFFFFFFFFF8h ; ~(0x8)
    and rcx, r9
    jz Copy4
    sub r8, rcx
    shr rcx, 3                 ; rcx /= 8
    rep movsq  
    jmp Copy1     
    
Copy4:
    mov rcx, r8   
    mov r9, 0FFFFFFFFFFFFFFF4h ; ~(0x4)
    and rcx, r9
    jz Copy2
    sub r8, rcx
    shr rcx, 2                 ; rcx /= 4
    rep movsd  
    jmp Copy1     

Copy2:
    mov rcx, r8     
    mov r9, 0FFFFFFFFFFFFFFF2h  ; ~(0x2)
    and rcx, r9
    jz Copy1
    sub r8, rcx   
    shr rcx, 1                  ; rcx /= 2
    rep movsw 
    jmp Copy1  

Copy1:
    mov rcx, r8     
    rep movsb      

    .beginepilog
    pop rdi
    pop rsi
    ret
MemCpy endp

; EXTERN_C void* MemMove(void* dst, const void* src, size_t count);
    end
