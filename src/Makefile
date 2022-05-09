CC=g++
CFLAGS=-I../include

output-file=../libdefucker.a
source-file= \
	defucker.cc \


all: $(source-file:.cc=.o)

%.o : %.cc
	$(CC) $^ -c -o $@ $(CFLAGS)

clear: $(source-file:.cc=.o)
	rm $^

install: $(source-file:.cc=.o)
	ar crv $(output-file) $^
