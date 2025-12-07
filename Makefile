ARGS?=

CC=gcc
CFLAGS=-g -O2
BINS=01a 01b 02a 02b 03a 03b 04a 04b 05a 05b

all: $(BINS)

.PHONY:
run_%: %
	@./$< $(ARGS)

.PHONY:
format:
	clang-format -i *.c

.PHONY:
clean:
	@$(RM) -rf $(BINS) *.dSYM *.swp

%: %.c
	$(CC) $(CFLAGS) $< -o $@
