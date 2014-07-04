#
# Generic compiles-it-all Makefile
#  Loni Nix <lornix@lornix.com>
#    Last modified **********
#
# adhere to a higher standard
CSTDCFLAGS=-std=gnu99
#
# save any options preset on cmd line
EXTFLAGS:=$(CFLAGS)
CFLAGS=
#
# lots of debugging stuff included
CFLAGS+=-ggdb3
#
# but no optimization by default
CFLAGS+=-O0
#
# warn about lots of things
CFLAGS+=-Wall -Wextra -Wunused
#
# but only include libs as needed
#CFLAGS+=-Wl,--as-needed
#
PRGFLAGS+=-Wl,-rpath=\$$ORIGIN -Wl,--enable-new-dtags
#
SOFLAGS+=-shared -fpic -Wl,--entry=ex1_main
#
# sometimes we want to see it all
#CFLAGS+=-save-temps -fverbose-asm
#
# and in glorious color!
CFLAGS+=-fdiagnostics-color=always
#
# don't really use LDFLAGS much
#LDFLAGS+=
#
# but need to list libraries needed
LIBS+=-ldl
#
CC:=gcc
#
.SUFFIXES:
.SUFFIXES: .c .o
#
.phony: all clean test
#
EXECS=main ex1.o
#
all: $(EXECS)

main: main.c
	$(CC) $(CSTDCFLAGS) $(CFLAGS) $(EXTFLAGS) $(LDFLAGS) -c $< -o $@.o $(LIBS)
	$(CC) $(CSTDCFLAGS) $(CFLAGS) $(EXTFLAGS) $(LDFLAGS) $(PRGFLAGS) $@.o -o $@ $(LIBS)

ex1.o: ex1.c
	$(CC) $(CSTDCFLAGS) $(CFLAGS) $(EXTFLAGS) $(LDFLAGS) $(SOFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f *.o $(EXECS)

test: $(EXECS)
	@echo
	@echo "Testing main program, which dlopen's ex1 for 'hello'"
	@echo
	./main
	@echo
	@echo "============================================"
	@echo
	@echo "Testing ex1 object, should show cmd line params"
	@echo
	./ex1.o alpha beta charlie delta echo
