CC=gcc
CFLAGS=-Wall
SOURCE=foo.c

TEST_EXPECTED=exp
TEST_ACTUAL=act
TEST_INPUT=input
TEST_CMP=diff

all: test

a.exe: $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE)

$(TEST_ACTUAL): a.exe $(TEST_INPUT)
	@echo "Collecting output..."
	cat $(TEST_INPUT) \
	| ./a.exe \
	| tr -d "\r" \
	> $(TEST_ACTUAL)

test: a.exe $(TEST_ACTUAL) $(TEST_INPUT) $(TEST_EXPECTED)
	@echo "Comparing..."
	@$(TEST_CMP) -u $(TEST_ACTUAL) $(TEST_EXPECTED) \
		&& echo "pass!"
