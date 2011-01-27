CC = gcc
CFLAGS = -Wall
TARGET = a.exe
TARGETSRC = rc.c
SRC = isort.c
OBJS = $(SRC:.c=.o)
HDRS = $(SRC:.c=.h)

TEST_EXPECTED=exp
TEST_ACTUAL=actual
TEST_INPUT=input
TEST_CMP=diff

all: test

$(TARGET): $(TARGETSRC) $(HDRS) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(TARGETSRC)

isort.o: isort.c isort.h
	$(CC) $(CFLAGS) -c isort.c

$(TEST_ACTUAL): $(TARGET) $(TEST_INPUT)
	@echo "Collecting output..."
	cat $(TEST_INPUT) \
	| ./$(TARGET) 2>&1 \
	| tr -d "\r" \
	> $(TEST_ACTUAL)

test: $(TARGEt) $(TEST_ACTUAL) $(TEST_INPUT) $(TEST_EXPECTED)
	@echo "Comparing..."
	@$(TEST_CMP) -u $(TEST_ACTUAL) $(TEST_EXPECTED) \
		&& echo "pass!"

clean:
	rm *.o $(TARGET)
