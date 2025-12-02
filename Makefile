CC=gcc
CFLAGS=-g -O2

BINS=01

all: $(BINS)

run_%: %
	@./$<

clean:
	@$(RM) -rf *.dSYM $(BINS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@
