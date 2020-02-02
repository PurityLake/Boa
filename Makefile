CC=gcc
EXEXCUTABLE=boa
CFLAGS=-Wall -pedantic -g
INCLUDES=-I./include
SRC_DIR=./src
SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(subst $(SRC_DIR)/,,$(SRCS:.c=.o))

%.o: $(SRC_DIR)/%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES)

all: $(OBJS)
	$(CC) -o $(EXEXCUTABLE) $^ $(CFLAGS) $(INCLUDES)

.PHONY: clean
clean:
	rm -f *.o
	rm -rf *.dSYM
	rm -f $(EXEXCUTABLE)