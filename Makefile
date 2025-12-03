ARGS?=

CC=gcc
CFLAGS=-g -O2
BINS=01a 01b 02a

all: $(BINS)

run_%: %
	@./$< $(ARGS)

clean:
	@$(RM) -rf *.dSYM $(BINS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@
