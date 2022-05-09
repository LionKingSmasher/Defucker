sub-dir= \
	src/ \

all: compile

compile:
	for i in $(sub-dir) ; do \
		make -C $$i;     \
	done			\

clear:
	for i in $(sub-dir) ; do \
		make -C $$i clear;     \
	done

install:
	make -C src/ install
