COMPILER=gcc

operations:
	@$(COMPILER) -c src/operations/*.c -Iinclude/ -o bin/operations.o

project:
	@$(COMPILER) -c src/operations/*.c -Iinclude/ -o bin/operations.o
	@$(COMPILER) -c src/encrypt/*.c -Iinclude/ -o bin/encrypt.o
	@$(COMPILER) -c src/decrypt/*.c -Iinclude/ -o bin/decrypt.o
	@$(COMPILER) -c src/roundKeys/*.c -Iinclude/ -o bin/keyOperations.o
	$(COMPILER) -c src/main.c -Iinclude/ -o bin/main.o
	@$(COMPILER) bin/operations.o bin/encrypt.o bin/decrypt.o bin/keyOperations.o bin/main.o -o AES