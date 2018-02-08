
all:
	cmake build
	make -Cbuild

test: all
	./build/runtests
