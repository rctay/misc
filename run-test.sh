#!/bin/sh
CC=gcc
SRC=foo.c

$CC -Wall -g $SRC && (
	cat input | ./a.exe \
	| tr -d "\r" \
	> actual && \
	diff -u actual exp && echo "pass!" || (
		echo "failed!";
		cat actual;
	)
)
