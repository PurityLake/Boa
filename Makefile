CC=gcc
EXEXCUTABLE=boa
CFLAGS=-Wall -pedantic
INCLUDES=-I./include
SRC_DIR=./src
OBJS=lexer.o parser.o object.o main.o

%.o: $(SRC_DIR)/%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -g

all: $(OBJS)
	$(CC) -o $(EXEXCUTABLE) $^ $(CFLAGS) $(INCLUDES) -g

.PHONY: clean
clean:
	rm -f *.o
	rm -rf *.dSYM
	rm -f $(EXEXCUTABLE)