nasm = nasm -felf64 -o 

main:  clean main.o dict.o lib.o 
	ld -o main main.o dict.o lib.o
	
dict.o: dict.asm
	$(nasm) $@ dict.asm
	
lib.o: lib.asm
	$(nasm) $@ lib.asm
	
main.o: main.asm
	$(nasm) $@ main.asm
	
clean:
	rm -f *.o main 
