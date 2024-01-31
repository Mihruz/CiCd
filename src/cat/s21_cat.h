#ifndef CAT_H
#define CAT_H

#include <getopt.h>
#include <stdio.h>

typedef struct opt {
  int b, e, n, s, t, v;
} opt_t;

int read_options(int argc, char **argv, opt_t *grep_options);
void read_file(int argc, char **argv, opt_t *cat_option);

#endif