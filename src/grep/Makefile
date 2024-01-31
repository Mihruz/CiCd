CC = gcc
FLAGS = -Wall -Wextra -Werror -std=c11 -pedantic

all: s21_grep style

s21_grep: s21_grep.c
	$(CC) $(FLAGS) s21_grep.c -o s21_grep

rebuild: clean s21_grep style

clean:
	rm -rf s21_grep

check:
	cppcheck --enable=all --suppress=missingIncludeSystem s21_grep.c

style:
	clang-format -n -style=Google *.c
	clang-format -n -style=Google *.h

test:
	sh test_func_grep.sh