%include "words.inc"
%define BUFFER_SIZE 256
%define stdout 1
%define stderr 2

section .data
err: db "Cannot find such word!", 0


section .text
extern find_word
extern read_word
extern print_string 
extern string_equals
extern string_length 
extern exit
global _start 
_start:
	sub rsp, BUFFER_SIZE	; at this stage I moved stack pointer to buffer size
    mov rdi, rsp		
    mov rsi, BUFFER_SIZE	; and I will write a word in stack 
	call 	read_word

	mov 	rdi, rax
	mov 	rax, start 		; pointer of dictionary start
	mov 	r10, rax 		; r10 - buffer to save last address of dictionary start
	call 	find_word
	mov 	rdi, rax 		; get 1 - all is good, or 0 - all is bad 
	cmp 	rdi, 0 
	je 		.error
	add 	rdi, 8 			; at this stage we have address of key 
	call 	string_length 
	add 	rdi, rax 
	add 	rdi, 1 			; at this stage we have address of value (word) 
	mov		r8, stdout		; move in r8 the file descriptor (stdout), because I have same func to write
	call 	print_string
	jmp 	exit
.error: 
	mov 	r8, stderr		; move in r8 the file descriptor (stderr) 
	mov 	rdi, err
	call	print_string

	mov 	rdi, -41		; rdi = exit code 
	call	exit
