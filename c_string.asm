    .data
    StrTokCurrentToken  qword ?
 
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
StrCat proc frame
    push rsi
    .pushreg rsi
    push rdi
    .pushreg rdi
    .endprolog

    mov r8, rcx

; Find the end of the dst
    mov rdi, rcx
    mov rcx, -1
    xor al, al
    repne scasb            ; while (al = *rdi++) {}
    dec rdi

; Append src
    mov rsi, rdx
@@:
    lodsb 
    mov byte ptr [rdi], al
    inc rdi
    test al, al
    jnz @B

    mov rax, r8

    .beginepilog
    pop rdi
    pop rsi
    ret
StrCat endp

; extern "C" char* StrNCat(char* dst, const char* src, size_t count);
StrNCat proc frame
    push rsi
    .pushreg rsi
    push rdi
    .pushreg rdi
    .endprolog

    mov r9, rcx
    test r8, r8
    jz Done

; Find the end of the dst
    mov rdi, rcx
    mov rcx, -1
    xor al, al
    repne scasb            ; while (al = *rdi++) {}
    dec rdi

; Append src
    mov rsi, rdx
@@:
    lodsb 
    mov byte ptr [rdi], al
    inc rdi
    test al, al
    jz Done
    dec r8
    jnz @B

Done:
    mov rax, r9

    .beginepilog
    pop rdi
    pop rsi
    ret
StrNCat endp

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
StrCmp proc
@@:
    mov al, byte ptr [rcx]
    mov r8b, byte ptr [rdx]
    sub al, r8b              ; al = lhs[i] - rhs[i]
    jnz @F                   ; al != 0
    test r8b, r8b            
    jz @F                    ; rhs[i] == '\0'
    inc rcx
    inc rdx
    jmp @B

@@:
    movsx eax, al            ; eax = static_cast<int>(al)
    ret
StrCmp endp

; extern "C" int StrNCmp(const char* lhs, const char* rhs, size_t count);
StrNCmp proc 
    xor al, al

@@:
    dec r8
    jz @F
    mov al, byte ptr [rcx]
    mov r9b, byte ptr [rdx]
    sub al, r9b              ; al = lhs[i] - rhs[i]
    jnz @F                   ; al != 0
    test r9b, r9b            
    jz @F                    ; rhs[i] == '\0'
    inc rcx
    inc rdx
    jmp @B

@@:
    movsx eax, al            ; eax = static_cast<int>(al)
    ret
StrNCmp endp

; extern "C" const char* StrChr(const char* str, int ch);
StrChr proc
    xor eax, eax

@@:
    mov al, byte ptr [rcx]
    cmp al, dl               ; '\0' can also be searched
    je Found
    test al, al
    jz Done
    inc rcx
    jmp @B

Found:  
    mov rax, rcx

Done:
    ret
StrChr endp

; extern "C" const char* StrRChr(const char* str, int ch);
StrRChr proc frame
    push rdi
    .pushreg rdi
    .endprolog

; Find the end of the dst
    mov rdi, rcx
    mov r8, rcx
    mov rcx, -1
    xor al, al
    repne scasb            ; while (al = *rdi++) {}
    dec rdi

    test dl, dl
    jz Found

    neg rcx
    dec rcx                ; rcx = strlen(str)

    pushfq
    std                    ; RFLAGS.DF = 1
    mov al, dl
    repne scasb
    popfq

    test rcx, rcx          ; Reached beginning of string
    jz NotFound
    inc rdi

Found:
    mov rax, rdi
    jmp Done  

NotFound:
    xor eax, eax

Done:
    .beginepilog
    pop rdi
    ret
StrRChr endp

; extern "C" size_t StrSpn(const char* dst, const char* src);
StrSpn proc frame
    push rsi
    .pushreg rsi
    .endprolog

    mov rsi, rcx
    xor ecx, ecx

DstLoop:                   ; while (dst[i] != '\0') { ... }
    lodsb 
    test al, al
    jz Done
    mov r8, rdx            ; r8 = src

SrcLoop:                   ; while (src[j] != '\0' && dsr[i] != src[j]) { j++; }
    mov r9b, byte ptr [r8]
    test r9b, r9b
    jz Done
    cmp al, r9b
    je @F
    inc r8
    jmp SrcLoop

@@:
    inc rcx
    jmp DstLoop
    
Done:
    mov rax, rcx

    .beginepilog
    pop rsi
    ret
StrSpn endp

; extern "C" size_t StrCSpn(const char* dst, const char* src);
StrCSpn proc frame
    push rsi
    .pushreg rsi
    .endprolog

    mov rsi, rcx
    xor ecx, ecx

DstLoop:                   ; while (dst[i] != '\0') { ... }
    lodsb 
    test al, al
    jz Done
    mov r8, rdx            ; r8 = src

SrcLoop:                   ; while (src[j] != '\0' && dst[i] != src[j]) { j++; }
    mov r9b, byte ptr [r8]
    test r9b, r9b
    jz @F
    cmp al, r9b
    je Done
    inc r8
    jmp SrcLoop

@@:
    inc rcx
    jmp DstLoop
    
Done:
    mov rax, rcx

    .beginepilog
    pop rsi
    ret
StrCSpn endp

; extern "C" const char* StrPbrk(const char* dst, const char* breakset);
StrPbrk proc frame
    push rsi
    .pushreg rsi
    .endprolog

    mov rsi, rcx

DstLoop:                   ; while (dst[i] != '\0') { ... }
    lodsb 
    test al, al
    jz NotFound
    mov r8, rdx            ; r8 = breakset

BreaksetLoop:              ; while (breakset[j] != '\0' && dst[i] != breakset[j]) { j++; }
    mov r9b, byte ptr [r8]
    test r9b, r9b
    jz DstLoop
    cmp al, r9b
    je Found
    inc r8
    jmp BreaksetLoop

Found:
    dec rsi
    mov rax, rsi
    jmp Done

NotFound:
    xor eax, eax
    
Done:
    .beginepilog
    pop rsi
    ret
StrPbrk endp

; extern "C" const char* StrStr(const char* haystack, const char* needle);
StrStr proc
; Check for empty string
    mov al, byte ptr [rcx]
    test al, al
    jnz RestartScan
    mov rax, rcx
    jmp Done

RestartScan:
    mov r8, rdx                   ; r8 = needle
    mov r10, rcx                  ; r10 = haystack
ContinueScan:
    mov r9b, byte ptr [r8]
    test r9b, r9b
    jz Found
    inc rcx
    mov al, byte ptr [rcx]        ; al = *haystack
    test al, al
    jz NotFound
    cmp al, r9b
    jne RestartScan
    inc r8
    jmp ContinueScan

Found:
    inc r10
    mov rax, r10
    jmp Done

NotFound:
    xor eax, eax

Done:
    ret
StrStr endp

; extern "C" char* StrTok(char* str, const char* delim);
StrTok proc frame
    push rsi
    .pushreg rsi
    .endprolog

    test rcx, rcx                  
    cmovz rcx, [StrTokCurrentToken]      ; if (!str) { str = StrTokCurrentToken; }

    mov rsi, rcx

; Skip all characters from delim
SkipLoop:
    lodsb
    test al, al
    jz NotFound
    mov r8, rdx            ; r8 = delim

ContainsDelimLoop:
    mov r9b, byte ptr [r8]
    test r9b, r9b
    jz Tokenize
    cmp al, r9b
    je SkipLoop
    inc r8
    jmp ContainsDelimLoop

; All delimiters were skipped
Tokenize:
    dec rsi
    mov r10, rsi

StrLoop:
    lodsb
    test al, al
    jz SingleToken
    mov r8, rdx           ; r8 = delim

SearchDelimLoop:
    mov r9b, byte ptr [r8]
    test r9b, r9b
    jz StrLoop
    cmp al, r9b
    je Found
    inc r8
    jmp SearchDelimLoop

Found:
    mov [StrTokCurrentToken], rsi   ; *StrTokCurrentToken - next pos after '\0'
    dec rsi  
    mov byte ptr [rsi], 0
    mov rax, r10
    jmp Done

SingleToken:
    dec rsi
    mov [StrTokCurrentToken], rsi    ; *StrTokCurrentToken == '\0'
    mov rax, r10
    jmp Done

NotFound:
    xor eax, eax

Done:
    .beginepilog
    pop rsi
    ret
StrTok endp

; extern "C" const void* MemChr(const void* ptr, int ch, size_t count);
MemChr proc frame
    push rdi
    .pushreg rdi
    .endprolog

    test r8, r8
    jz NotFound

    mov rdi, rcx
    mov rcx, r8
    mov al, dl
    repne scasb 
    jnz NotFound

Found:
    dec rdi
    mov rax, rdi
    jmp Done

NotFound:
    xor eax, eax
    
Done:
    .beginepilog
    pop rdi
    ret
MemChr endp

; extern "C" int MemCmp(const void* lhs, const void* rhs, size_t count);
MemCmp proc frame
    push rsi
    .pushreg rsi
    push rdi
    .pushreg rdi
    .endprolog

    xor eax, eax
    test r8, r8
    jz Done

    mov rsi, rcx
    mov rdi, rdx
    mov rcx, r8
    repe cmpsb
    jz Done

    mov al, byte ptr [rsi - 1]
    sub al, byte ptr [rdi - 1]
    movsx eax, al

Done:
    .beginepilog
    pop rdi
    pop rsi
    ret
MemCmp endp

; extern "C" void* MemSet(void* dst, int ch, size_t count)
MemSet proc frame
    push rdi
    .pushreg rdi
    .endprolog

    mov rdi, rcx     ; rdi = dst
    mov r11, rcx
    movzx eax, dl
    movzx r9, dl

; Dispatch by size
    mov rcx, r8
    and rcx, -8     ; 0xFFFFFFFF'FFFFFFF8
    jnz Set8
    and rcx, -12    ; 0xFFFFFFFF'FFFFFFF4
    jnz Set4
    and rcx, -14    ; 0xFFFFFFFF'FFFFFFF2
    jnz Set2
    jmp Set1

Set8:
    sub r8, rcx
    shr rcx, 3                 ; rcx /= 8
    mov r10d, 7

@@:
    shl r9, 8
    or rax, r9
    dec r10d
    jnz @B

    rep stosq    
    jmp Set1

Set4:
    sub r8, rcx
    shr rcx, 2                 ; rcx /= 4
    mov r10d, 3

@@:
    shl r9, 8
    or rax, r9
    dec r10d
    jnz @B

    rep stosd   
    jmp Set1

Set2:
    sub r8, rcx
    shr rcx, 1                 ; rcx /= 2
    shl r9, 8
    or rax, r9
    rep stosw  
    jmp Set1 

Set1:
    mov rcx, r8
    rep stosb 

    mov rax, r11

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

; Dispatch by size
    mov rcx, r8
    and rcx, -8     ; 0xFFFFFFFF'FFFFFFF8
    jnz Copy8
    and rcx, -12    ; 0xFFFFFFFF'FFFFFFF4
    jnz Copy4
    and rcx, -14    ; 0xFFFFFFFF'FFFFFFF2
    jnz Copy2
    jmp Copy1

Copy8:  
    sub r8, rcx
    shr rcx, 3                 ; rcx /= 8
    rep movsq  
    jmp Copy1     
    
Copy4:
    sub r8, rcx
    shr rcx, 2                 ; rcx /= 4
    rep movsd  
    jmp Copy1     

Copy2:
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
MemMove proc
MemMove endp
    end
