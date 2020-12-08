COMPILER=gcc

operations:
	@$(COMPILER) -c src/operations/*.c -Iinclude/ -o bin/operations.o

project:
	$(COMPILER) -c src/main.c -Iinclude/ -o bin/main.o
	@$(COMPILER) bin/operations.o bin/main.o -o AES