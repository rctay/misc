CC = gcc
CFLAGS = -pg
TARGET = insertionSort
TARGETSRC = insertionSort.c
SRC = genrandom.c testfuncs.c isort.c htable.c
OBJS = $(SRC:.c=.o)
HDRS = $(SRC:.c=.h)


$(TARGET): $(TARGETSRC) $(HDRS) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(TARGETSRC) -o $@

genrandom.o: genrandom.c genrandom.h
	$(CC) -c genrandom.c

testfuncs.o: testfuncs.c testfuncs.h htable.h
	$(CC) -c testfuncs.c

isort.o: isort.c isort.h
	$(CC) $(CFLAGS) -c isort.c

htable.o: htable.c htable.h
	$(CC) -c htable.c

clean:
	rm *.o $(TARGET)
