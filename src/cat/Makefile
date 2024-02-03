CFLAGS = -Werror -Wextra -Wall
CC = gcc

all : s21_cat
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat 

check:
	clang-format -style=google -n *.c *.h

clean:
	rm -rf s21_cat

rebuild:
	make clean
	make all