;VELC CODEGEN START
section .data
hello db "uwILOVEVEL u", 0
hell db 3330
;Assembly start

    meow db "I love vel", 10
    meowll equ $ - meow

;Assembly end

section .bss
resu resq 1

section .text
    global _start
_start:
;VEL standard library
;    I love vel
;comment test! i love vel!
;Assembly start


    mov rax, 1          ; sys_write
    mov rdi, 1          ; stdout
    mov rsi, meow       ; message address
    mov rdx, meowll     ; message length
    syscall

    mov rax, 60         ; sys_exit
    xor rdi, rdi        ; exit code 0
    syscall



;Assembly end
;    bandit1 password: ZjLjTmM6FvvyRnrb2rfNWOZOTa6ip5If
;    bandit2 password: 263JGJPfgU6LtdEvgfWU1XP5yac29mFx
;    bandit3 password: MNk8KNH3Usiio41PRUEoDFPqfxLPlSmx
;    bandit4 password: 2WmrDFRmJIq3IPxneAaMGhap0pFhF3NJ
;    bandit5 password: 4oQYVPkxZOOEOO5pTW81FB8j8lxXGUQw
;    bandit6 password: HWasnPhtq9AVKe0dmk45nxy20cvUa6EG
;    bandit7 password: morbNTDkSW6jIlUc0ymOdMaLnOlFVAaj
;    bandit8 password: dfwvzFQi4mU0wfNbFOe9RoWskMLg7eEc
;    

;VELC CODEGEN END
