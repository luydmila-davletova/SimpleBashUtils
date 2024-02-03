CFLAGS = -Werror -Wextra -Wall
CC = gcc

all : s21_grep
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep

check:
	clang-format -style=google -n *.c *.h

clean:
	rm -rf s21_grep s21_cat

rebuild:
	make clean
	make all