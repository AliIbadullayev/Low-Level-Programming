section .text 
global find_word
extern string_equals

find_word:
.loop:
	add rax, 8
	mov rsi, rax
	call string_equals
	cmp rax, 0
	je .bad
	jmp .good
.good: 
	mov rax, r10
	ret
.bad:
	cmp qword [r10], 0
	je .last_word
	mov rax, [r10]
	mov r10, rax
	mov rdi, rdx
	jmp .loop
	
.last_word:
	mov rax, 0
	ret
