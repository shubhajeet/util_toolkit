# 
# Makefile
#
# IOE PULCHOWK
# 2070 BEX cproject
# 
# @authors shubhajeet
# @authors ccr

# compiler to use
CC = gcc

# flags to pass compiler
CFLAGS = -ggdb

# name for executable 
EXE = util

# space-separated list of header files
HDRS = recover.h comprez.h

# space-separated list of libraries perfixed with -l
LIBS = -lm

# space-separated list of source files
SRCS = util.c recover.c comprez.c hashstruct.c

# auotmatically generated list of object files
OBJS = $(SRCS:.c=.o)

# default target
$(EXE):$(OBJS) $(HDRS) Makefile
	$(CC) $(CFLAGS) -o $(EXE) $(OBJS) bytecount.o $(LIBS)

# dependencies
$(OBJS):$(HDRS)

# dependencies for comprez library

comprez.o:bytecount.o hashstruct.o

bytecount.o:bytecount.h bytecount.c

hashstruct.o:hashstruct.h hashstruct.c

# creation of dynamic library
# a trial to create apache module
.PHONY: lib
lib: comprez.c hashstruct.c bytecount.c
	gcc -fPIC -shared -c comprez.c -o lib/comprez.o
	gcc -fPIC -shared -c hashstruct.c -o lib/hashstruct.o
	gcc -fPIC -shared -c bytecount.c -o lib/bytecount.o
	gcc -shared -Wl -o lib/libutil.so lib/comprez.o lib/bytecount.o lib/hashstruct.o

# for the propose of testing
.PHONY: test-recovery test-comprez test-decomprez cleanall cleantest

#testing recovery
test-recovery:
	./util -v -i test/card.raw -p test

#testing comprez
test-comprez:
	./util -v -c test/sample.txt -f test/sample_word_lb.cmpz -m w -a lb
	./util -v -c test/sample.txt -f test/sample_word_p.cmpz -m w -a p
	./util -v -c test/sample.txt -f test/sample_ascii_lb.cmpz -m a -a lb
	./util -v -c test/sample.txt -f test/sample_ascii_p.cmpz -m a -a p


#testing decomprez
test-decomprez:
	./util -v -d test/sample_word_lb.cmpz -m w
	./util -v -d test/sample_word_p.cmpz -m w
	./util -v -d test/sample_ascii_lb.cmpz -m a
	./util -v -d test/sample_ascii_p.cmpz -m a

# cleaning all compiled file
cleanall:
	rm -f core $(EXE) *.o

# cleaning recovery test
cleantest:
	rm -r -f test/tmp*
	rm -r -f test/*.cmpz

# PROOF OF CONECPT
# USE OF COMPREZ IN NETWORKING VIA NETCAT
.PHONY: start-server request-file

start-server:
	sudo nc -v -l 230 < test/sample_word_lb.cmpz

request-file:
	nc -v 127.0.0.1 230 > test/file_from_server_w_lb.cmpz
	./util -d test/file_from_server_w_lb.cmpz -m w