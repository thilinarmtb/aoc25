CC=gcc
CFLAGS=-g -O2
BINS=01a

all: $(BINS)

run_%: %
	@./$<

clean:
	@$(RM) -rf *.dSYM $(BINS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@
