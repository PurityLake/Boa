CC=gcc
EXEXCUTABLE=boa
CFLAGS=-Wall -pedantic
INCLUDES=-I./include
SRC_DIR=./src
OBJS=lexer.o main.o parser.o

%.o: $(SRC_DIR)/%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES)

all: $(OBJS)
	$(CC) -o $(EXEXCUTABLE) $^ $(CFLAGS) $(INCLUDES)

.PHONY: clean
clean:
	rm -f *.o
	rm -rf *.dSYM
	rm -f $(EXEXCUTABLE)