CC=gcc -Wall
CFLAGS= -g

all: bmpcreator test_bmp_create test_bmp_size test_bmp_rewrite

bmpcreator: bmpcreator.o tinybmp.o
	$(CC) $(CFLAGS) -o bmpcreator bmpcreator.o tinybmp.o

bmpcreator.o: bmpcreator.c
	$(CC) $(CFLAGS) -c bmpcreator.c

tinybmp.o: tinybmp.c
	$(CC) $(CFLAGS) -c tinybmp.c

test_bmp_create: test_bmp_create.o tinybmp.o
	$(CC) $(CFLAGS) -o test_bmp_create test_bmp_create.o tinybmp.o

test_bmp_size: test_bmp_size.o tinybmp.o
	$(CC) $(CFLAGS) -o test_bmp_size test_bmp_size.o tinybmp.o

test_bmp_rewrite: test_bmp_rewrite.o tinybmp.o
	$(CC) $(CFLAGS) -o test_bmp_rewrite test_bmp_rewrite.o tinybmp.o

clean:
	rm -fR *.o
