ARGS?=

CC=gcc
CFLAGS=-g -O2
BINS=01a 01b 02a

all: $(BINS)

run_%: %
	@./$< $(ARGS)

format:
	clang-format -i *.c

clean:
	@$(RM) -rf $(BINS) *.dSYM *.swp

%: %.c
	$(CC) $(CFLAGS) $< -o $@
