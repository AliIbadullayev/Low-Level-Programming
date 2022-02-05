global exit
global string_length
global print_string
global print_char
global print_newline
global print_uint
global print_int
global string_equals
global read_char
global read_word
global parse_int
global parse_uint 
global string_copy
global print_string_stderr

; Принимает код возврата и завершает текущий процесс
exit: 
    	mov rax, 60 
    	syscall 
   

; Принимает указатель на нуль-терминированную строку, возвращает её длину
string_length:
    	mov rax, 0
.loop:	
		cmp byte [rdi + rax] , 0 
    	je .end
   		inc rax
   		jmp .loop
   	
.end:   
		ret

; Принимает указатель на нуль-терминированную строку, выводит её в stdout/stderr
print_string:
    	call string_length
    	mov rdx, rax
    	mov rsi, rdi 
    	mov rdi, r8 
    	mov rax, 1
    	syscall
		call print_newline
    	ret

; Принимает код символа и выводит его в stdout
print_char:
		push rdi 
		mov rsi, rsp
    	mov rax, 1 
   		mov rdx, 1 
    	mov rdi, 1
    	syscall 
    	add rsp, 8
    	ret

; Переводит строку (выводит символ с кодом 0xA)
print_newline:
    	mov rax, 1 
    	mov rdx, 0xA
    	push rdx
    	mov rsi, rsp
    	mov rdx, 1 
    	mov rdi, 1 
    	syscall 
    	add rsp, 8
    	ret

; Выводит беззнаковое 8-байтовое число в десятичном формате 
; Совет: выделите место в стеке и храните там результаты деления
; Не забудьте перевести цифры в их ASCII коды.
print_uint:
    	mov rax, rdi
    	mov r10, 10
    	push 0
.div:
        xor rdx, rdx
        div r10
        add rdx, '0'
        push rdx
        cmp rax, 0
        je .print
        jmp .div
.print:
        pop rdi
        cmp rdi, 0
        je .end
        call print_char
        jmp .print
.end:
        ret         


; Выводит знаковое 8-байтовое число в десятичном формате 
print_int:
    	cmp rdi, 0
	jl .negative
	jmp .positive
.positive:
        call print_uint
        jmp .end   
.negative:
    	neg rdi
    	mov r8, rdi 
    	mov rdi, '-'
    	call print_char
    	mov rdi, r8 
    	call print_uint
.end:
        ret         


; Принимает два указателя на нуль-терминированные строки, возвращает 1 если они равны, 0 иначе
string_equals:
    
    	call string_length
    	mov r8, rax
    	mov rdx, rdi; move the address of first string to rdx
    	mov rdi, rsi 
    	call string_length 
    	mov r9, rax
    	cmp r8, r9 ; check that the lengths of string is equals 
    	je .check_letters 
    	jmp .bad
.check_letters:
    	mov rsi, rdi
    	mov rdi, rdx
    	mov rax, 0
    
.loop:
    	mov cl, byte [rdi + rax]
    	cmp cl, byte [rsi+rax]
    	jne .bad
    	inc rax
    	cmp r8, 0 ; if string is null
    	je .good
    	dec r8
    	cmp r8, 0
    	je .good
    	jmp .loop
.good:
    	mov rax, 1
    	ret
.bad:
    	mov rax, 0
    	ret

; Читает один символ из stdin и возвращает его. Возвращает 0 если достигнут конец потока
read_char:
    	mov rax, 0
    	push 0 
    	mov rsi, rsp
    	mov rdi, 0
    	mov rdx, 1
    	syscall
    	pop rax
    	cmp rax, 0 
    	je .return 
    	ret
.return:
	mov rax, 0
    	ret 

; Принимает: адрес начала буфера, размер буфера
; Читает в буфер слово из stdin, пропуская пробельные символы в начале, .
; Пробельные символы это пробел 0x20, табуляция 0x9 и перевод строки 0xA.
; Останавливается и возвращает 0 если слово слишком большое для буфера
; При успехе возвращает адрес буфера в rax, длину слова в rdx.
; При неудаче возвращает 0 в rax
; Эта функция должна дописывать к слову нуль-терминатор

read_word:
	mov rax, 0 
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, 0 
	syscall 
	mov byte [rsi+rax], 0 
	mov r8, 0
	mov r10, rdx
	
.check_space:
	cmp byte [rsi+r8], 32 
	je .delete
	cmp byte [rsi+r8], 9
	je .delete
	cmp byte [rsi+r8], 10
	je .delete
	jmp .word
	
;making offset for spaces 
.delete:
	inc r8
	dec r10
	jmp .check_space
.word:
	add rsi, r8 
	mov rdi, rsi 
	
; Counting length of string 
    	mov rax, 0

.loop:	
	cmp byte [rdi + rax] , 0
    	je .end
   	inc rax
   	jmp .loop
   	
; if length of string bigger than length of buffer, then return 0 into rax, else in rdx - length of string, rax - buffer address
.end:	
	sub rax, 1
	mov byte [rdi + rax ] , 0
	cmp rax, rdx
    	jg .bad
    	jmp .good
.bad:
    	mov rax, 0
    	ret
.good:
	mov rdx, rax
	mov rax, rsi 
	ret
 

; Принимает указатель на строку, пытается
; прочитать из её начала беззнаковое число.
; Возвращает в rax: число, rdx : его длину в символах
; rdx = 0 если число прочитать не удалось
parse_uint:
	call string_length
	mov r8, 0
	mov r9, 0
.loop:
   	cmp byte [rdi+r8], 48 ; more than '0'
   	jge .second ; second test, that show that char is lower than '9'
   	jmp .bad
.second:
	cmp byte [rdi+r8], 58
	jle .good
	jmp .bad 
.good:
	inc r8 
	mov al, byte [rdi + r8 -1]
	and rax, 0xff
	sub rax, '0' 
	mov rdx, 0
	mov r10, 10
	add r9, rax
	mov rax, r9
	mul r10
	mov r9, rax
	jmp .loop
.bad:
	mov rax, r9
	mov rdx, 0
	mov r10, 10
	div r10
	mov rdx, r8
   	ret


; Принимает указатель на строку, пытается
; прочитать из её начала знаковое число.
; Если есть знак, пробелы между ним и числом не разрешены.
; Возвращает в rax: число, rdx : его длину в символах (включая знак, если он был) 
; rdx = 0 если число прочитать не удалось
parse_int:
    	mov r8, 0
	mov r9, 0
.loop:
   	cmp byte [rdi+r8], 45 ; 
   	je .negative 
   	jmp .positive
.negative:
	add rdi, 1
	call parse_uint 
	neg rax
	add rdx, 1
	ret
.positive:
	call parse_uint 
    	ret 

; Принимает указатель на строку, указатель на буфер и длину буфера
; Копирует строку в буфер
; Возвращает длину строки если она умещается в буфер, иначе 0
string_copy:
    	call string_length
    	cmp rax, rdx
    	jge .bad
    	cmp rax, rdx 
    	jmp .good
.bad:
    	mov rax, 0
    	ret
.good:
	mov r8, 0
	mov r9, rax
.loop:
	cmp r9, 0
	mov rax, [rdi+r8]
	mov byte[rsi+r8], al
	inc r8
	dec r9
	cmp r9, 0
	je .end
	jmp .loop
.end:	
	mov byte[rsi+r8], 0
	ret
