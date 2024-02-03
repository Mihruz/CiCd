#include "s21_grep.h"

int main(int argc, char **argv) {
  opt_t grep_options = {0};
  if (argc > 2) {
    char pattern[SIZE] = {0};
    if (read_options(argc, argv, &grep_options, pattern)) {
      int current_file = optind;
      if (pattern[0] == 0) {
        strcat(pattern, argv[optind]);
        current_file++;
      }
      if (argv[current_file + 1] != NULL) grep_options.multiple_files = 1;
      while (current_file < argc) {
        grep_options.filename = argv[current_file];
        read_file(argv, &grep_options, pattern);
        current_file++;
      }
    }
  } else
    printf(ERROR);
  return 0;
}

int read_options(int argc, char **argv, opt_t *grep_options, char *pattern) {
  int option, check = 1;
  int pattern_count = 0;
  static struct option long_options[] = {{0, 0, 0, 0}};
  while ((option = getopt_long(argc, argv, "e:ivclnhsf:o", long_options,
                               NULL)) != -1) {
    if (option == 'e') {
      grep_options->e = 1;
      if (pattern_count) strcat(pattern, "|");
      strcat(pattern, optarg);
      pattern_count++;
    } else if (option == 'i')
      grep_options->i = 1;
    else if (option == 'v')
      grep_options->v = 1;
    else if (option == 'c')
      grep_options->c = 1;
    else if (option == 'l')
      grep_options->l = 1;
    else if (option == 'n')
      grep_options->n = 1;
    else if (option == 'h')
      grep_options->h = 1;
    else if (option == 's')
      grep_options->s = 1;
    else if (option == 'f') {
      grep_options->f = 1;
      f_pattern(optarg, pattern);
    } else if (option == 'o')
      grep_options->o = 1;
    else if (option == '?')
      check = 0;
  }
  return check;
}

void read_file(char **argv, opt_t *grep_options, char *pattern) {
  grep_options->match_lines = 0;
  grep_options->current_line = 1;

  FILE *fp = fopen(grep_options->filename, "r");
  if (fp != NULL) {
    char text[SIZE] = {0};
    int cflags = REG_EXTENDED;
    regex_t reg;
    if (grep_options->i) cflags = REG_ICASE;
    regcomp(&reg, pattern, cflags);
    regmatch_t pmatch[1];
    while (fgets(text, 4096, fp) != NULL) {
      int reg_result = regexec(&reg, text, 1, pmatch, 0);
      if (strchr(text, '\n') == NULL) strcat(text, "\n");
      if ((reg_result == 0 && !grep_options->v) ||
          (reg_result == REG_NOMATCH && grep_options->v)) {
        grep_options->match = 1;
        grep_options->match_lines++;
        print_grep(grep_options, text);
      }
      if (grep_options->o && !grep_options->c && !grep_options->l &&
          !grep_options->v) {
        while (regexec(&reg, text, 1, pmatch, 0) == 0) {
          for (int i = 0; i < pmatch->rm_eo; i++) {
            if (i >= pmatch->rm_so) printf("%c", text[i]);
            text[i] = 127;
          }
          printf("\n");
        }
      }
      grep_options->current_line++;
    }
    if (grep_options->l && grep_options->match_lines)
      printf("%s\n", grep_options->filename);
    if (grep_options->c) {
      if (grep_options->multiple_files) printf("%s:", grep_options->filename);
      printf("%d\n", grep_options->match_lines);
    }
    regfree(&reg);
    fclose(fp);
  } else if (!grep_options->s)
    fprintf(stderr, "%s: %s: No such file or directory\n", argv[0],
            grep_options->filename);
}

void print_grep(opt_t *grep_options, char *text) {
  if (grep_options->multiple_files && !grep_options->h && !grep_options->l &&
      !grep_options->c)
    printf("%s:", grep_options->filename);
  if (grep_options->n) printf("%d:", grep_options->current_line);
  if (grep_options->match && !grep_options->c && !grep_options->l &&
      !grep_options->o) {
    printf("%s", text);
  }
}

void f_pattern(char *file_pattern, char *pattern) {
  FILE *fp = fopen(file_pattern, "r");
  if (fp != NULL) {
    int i = 0;
    int curr_ch;
    while ((curr_ch = getc(fp)) != EOF) {
      if (curr_ch == '\n') curr_ch = '|';
      pattern[i] = (char)curr_ch;
      i++;
    }
    if (pattern[i - 1] == '|') pattern[i - 1] = '\0';
    fclose(fp);
  }
}