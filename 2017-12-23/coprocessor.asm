bits 64

section     .text
global      _start 
_start: 
    mov     rdx,len   
    mov     rcx,msg   
    mov     rbx,1   
    mov     rax,4   
    int     0x80   
    mov     rax,1  
    int     0x80   

	mov rbx, 84
	mov rcx, rbx
	test rax, rax
	jnz line05
	jmp line09
line05:
	imul rbx, 100
	sub rbx, -100000
	mov rcx, rbx
	sub rcx, -17000
line09:
	mov r9, 1
	mov rdx, 2
line11:
	mov r8, 2
line12:
	mov r10, rdx
	imul r10, r8
	sub r10, rbx
	jnz line17
	mov r9, 0
line17:
	sub r8, -1
	mov r10, r8
	sub r10, rbx
	jnz line12
	sub rdx, -1
	mov r10, rdx
	sub r10, rbx
	jnz line11
	test r9, r9
	jnz line27
	sub r11, -1
line27:
	mov r10, rbx
	sub r10, rcx
	test r10, r10
	jnz line31
	jmp end
line31:
	sub rbx, -17
	jmp line09
end:
	ret

section     .data
msg     db  'Hello world',0xa  
len     equ $ - msg
