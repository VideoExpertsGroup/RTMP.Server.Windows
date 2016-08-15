
all:
	i686-w64-mingw32-g++ src/main_test.c -D__STDC_CONSTANT_MACROS -I./include -L./lib/ \
		-lrtmpserver -Wl,-Bstatic -pthread -static-libgcc -o ./bin/main_test.exe
