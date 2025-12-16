;VELC CODEGEN START
section .data
;Assembly start

    meow db "I love vel", 10
    meowll equ $ - meow

;Assembly end

section .bss

section .text
    global _start
_start:
;VEL standard library
;    I love vel
;comment test! i love vel!
;Assembly start

    push rbp
    mov rbp, rsp

    mov rax, 1          ; sys_write
    mov rdi, 1          ; stdout
    mov rsi, meow       ; message address
    mov rdx, meowll     ; message length
    syscall

    mov rsp, rbp
    pop rbp

    mov rax, 60         ; sys_exit
    xor rdi, rdi        ; exit code 0
    syscall


;Assembly end

;VELC CODEGEN END
