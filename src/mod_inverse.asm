section .text
global modInverse

; Function: modInverse(long a, long m)
; Arguments (System V AMD64 ABI):
;   rdi = a (the number to invert)
;   rsi = m (the modulus)
; Returns:
;   rax = modular inverse of a mod m, or -1 if not exists.

modInverse:
    ; Prologue not strictly needed for leaf function, but good practice
    push rbp
    mov rbp, rsp

    ; Variables mapping to registers:
    ; t     = rcx
    ; new_t = r8
    ; r     = r9  (rem)
    ; new_r = r10 (new_rem)
    
    ; Initialize: t=0, new_t=1
    xor rcx, rcx        ; t = 0
    mov r8, 1           ; new_t = 1
    
    ; Initialize: r=m, new_r=a
    mov r9, rsi         ; r = m
    mov r10, rdi        ; new_r = a

.loop:
    cmp r10, 0          ; while (new_r != 0)
    je .end_loop

    ; Calculate quotient = r / new_r
    ; idiv divides rdx:rax by operand. Result in rax, Remainder in rdx
    mov rax, r9         ; Move r into rax for division
    cqo                 ; Sign extend rax into rdx
    idiv r10            ; rax = r / new_r, rdx = remainder
    mov r11, rax        ; r11 = quotient

    ; --- Update t and new_t ---
    ; temp_t = t (saved in rbx just to move)
    ; t = new_t
    ; new_t = temp_t - quotient * new_t
    
    mov rbx, rcx        ; temp_t = t
    mov rcx, r8         ; t = new_t
    
    ; Calculate quotient * new_t
    imul r11, r8        ; r11 = quotient * new_t
    
    ; new_t = temp_t - (quotient * new_t)
    sub rbx, r11        
    mov r8, rbx         ; new_t = result

    ; --- Update r and new_r ---
    ; temp_r = r
    ; r = new_r
    ; new_r = temp_r - quotient * new_r
    ; Note: We already have the remainder from the idiv instruction in rdx!
    ; new_r becomes the remainder. r becomes the old new_r.
    
    mov r9, r10         ; r = old new_r
    mov r10, rdx        ; new_r = remainder (from previous idiv)

    jmp .loop

.end_loop:
    ; If r > 1, not invertible
    cmp r9, 1
    jg .error

    ; If t < 0, t = t + m
    cmp rcx, 0
    jge .return_result
    add rcx, rsi        ; t = t + m

.return_result:
    mov rax, rcx        ; Return t
    jmp .exit

.error:
    mov rax, -1         ; Return -1 indicating failure

.exit:
    leave
    ret
