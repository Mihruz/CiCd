#include "s21_cat.h"

int main(int argc, char **argv) {
  opt_t cat_option = {0};
  if (read_options(argc, argv, &cat_option)) {
    read_file(argc, argv, &cat_option);
  }
  return 0;
}

int read_options(int argc, char **argv, opt_t *cat_option) {
  int option, optCheck = 1;
  const struct option long_options[] = {{"number-nonblank", 0, 0, 'b'},
                                        {"number", 0, 0, 'n'},
                                        {"squeeze-blank", 0, 0, 's'},
                                        {0, 0, 0, 0}};

  while ((option = getopt_long(argc, argv, "+benstvET", long_options, NULL)) !=
         -1) {
    if (option == 'b')
      cat_option->b = 1;
    else if (option == 'e') {
      cat_option->e = 1;
      cat_option->v = 1;
    } else if (option == 'n')
      cat_option->n = 1;
    else if (option == 's')
      cat_option->s = 1;
    else if (option == 't') {
      cat_option->t = 1;
      cat_option->v = 1;
    } else if (option == 'E')
      cat_option->e = 1;
    else if (option == 'T')
      cat_option->t = 1;
    else if (option == 'v')
      cat_option->v = 1;
    else {
      optCheck = 0;
    }
  }
  return optCheck;
}

void read_file(int argc, char **argv, opt_t *cat_option) {
  int currentFile = optind;
  int counter, blank_string;
  while (currentFile < argc) {
    if (argc > 1) {
      FILE *fp = fopen(argv[currentFile], "r");
      if (fp != NULL) {
        counter = 1;
        blank_string = 0;
        char curr_ch, prev_ch = '\n';
        while ((curr_ch = getc(fp)) != EOF) {
          if (cat_option->s && prev_ch == '\n' && curr_ch == '\n') {
            blank_string++;
            if (blank_string > 1) continue;
          } else
            blank_string = 0;
          if (cat_option->b && prev_ch == '\n' && curr_ch != '\n') {
            printf("%6d\t", counter);
            counter++;
          }
          if (cat_option->n && prev_ch == '\n' && cat_option->b == 0) {
            printf("%6d\t", counter);
            counter++;
          }
          if (cat_option->e && curr_ch == '\n') printf("$");
          if (cat_option->t && curr_ch == '\t') {
            printf("^");
            curr_ch = 'I';
          }
          if (cat_option->v) {
            if ((curr_ch >= 0 && curr_ch <= 31) && curr_ch != '\n' &&
                curr_ch != '\t') {
              printf("^");
              curr_ch += 64;
            } else if (curr_ch == 127) {
              printf("^");
              curr_ch -= 64;
            }
          }
          prev_ch = curr_ch;
          printf("%c", curr_ch);
        }
        fclose(fp);
      } else
        fprintf(stderr, "%s: %s: No such file or directory\n", argv[0],
                argv[currentFile]);
      currentFile++;
    }
  }
}
