
all: build

build:
	cmake build
	make -Cbuild

test: build
	./build/runtests

.PHONY: all build test
