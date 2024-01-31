#ifndef GREP_H
#define GREP_H
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 4096
#define ERROR                                                              \
  "usage: grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "         \
  "[-C[num]]\n"                                                            \
  "        [-e pattern] [-f file] [--binary-files=value] [--color=when]\n" \
  "        [--context[=num]] [--directories=action] [--label] "            \
  "[--line-buffered]\n"                                                    \
  "        [--null] [pattern] [file ...]\n"

typedef struct opt {
  int e, i, v, c, l, n, h, s, f, o;
  int current_line;
  int multiple_files;
  int match;
  int match_lines;
  char *filename;
} opt_t;

int read_options(int argc, char **argv, opt_t *grep_options, char *pattern);
void read_file(char **argv, opt_t *grep_options, char *pattern);
void print_grep(opt_t *grep_options, char *text);
void f_pattern(char *file_pattern, char *pattern);

#endif