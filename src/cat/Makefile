CC = gcc
FLAGS = -Wall -Wextra -Werror -std=c11 -pedantic

all: s21_cat style

s21_cat: s21_cat.c
	$(CC) $(FLAGS) s21_cat.c -o s21_cat

rebuild: clean s21_cat style

clean:
	rm -rf s21_cat

check:
	cppcheck --enable=all --suppress=missingIncludeSystem s21_cat.c

style:
	clang-format -n -style=Google *.c
	clang-format -n -style=Google *.h

test:
	sh test_func_cat.sh