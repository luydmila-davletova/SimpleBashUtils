#include "s21_grep.h"

#include <string.h>

int main(int argc, char *argv[]) {
  print_file_non_flags(argc, argv);
  return 0;
}

void print_file_non_flags(int argc, char *argv[]) {
  regex_t regex;
  if (argc == 1) {
    fprintf(stderr,
            "usage: grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
            "[-C[num]]\n");
    fprintf(stderr,
            "\t[-e pattern] [-f file] [--binary-files=value] [--color=when]\n");
    fprintf(stderr,
            "\t[--context[=num]] [--directories=action] [--label] "
            "[--line-buffered]\n");
    fprintf(stderr, "\t[--null] [pattern] [file ...]\n");
  } else if (argc == 2) {
    char current_symbol = getchar();
    while (current_symbol != EOF) {
      current_symbol = getchar();
    }
  } else if ((argc >= 3)) {
    grep_read_flags(argc, argv, &options, &regex);
    grep_read_and_print_flags(argc, argv, &options, &regex);
  }
}

void grep_read_flags(int argc, char *argv[], opt *options, regex_t *regex) {
  int opt = 0;
  int reg_flag = 0;
  char pattern[1024] = {0};
  while ((opt = getopt_long(argc, argv, "e:ivclnsoh", NULL, NULL)) !=
         (-1)) {  // NULL передается в качестве значений longopts и longindex,
                  // поскольку эти параметры не используются в данном контексте.
    switch (opt) {
      case 'i':
        options->i_flag = 1;
        reg_flag = REG_ICASE;  // игнорирует регистр
        break;
      case 'v':
        options->v_flag = 1;
        break;
      case 'c':
        options->c_flag = 1;
        break;
      case 'l':
        options->l_flag = 1;
        break;
      case 'n':
        options->n_flag = 1;
        break;
      case 'h':
        options->h_flag = 1;
        break;
      case 's':
        options->s_flag = 1;
        break;
      case 'e':
        options->e_flag = 1;
        strcat(pattern, optarg);  // конкатенация строк
        strcat(pattern, "|");
        if (reg_flag == 0) {
          reg_flag = 1;
        }
        pattern[strlen(pattern) - 1] = '\0';
        break;
    }
  }
  if (options->e_flag) {
    regcomp(regex, pattern, reg_flag);
  } else {
    regcomp(regex, argv[optind], reg_flag);
    optind++;
  }
}

void grep_read_and_print_flags(int argc, char *argv[], opt *options,
                               regex_t *regex) {
  FILE *file;
  char *line = NULL;
  size_t len_str = 0;
  int count_read_symbols = 0;
  int status = 0;
  int str_count = 0;
  int str_count_c = 0;
  int count_names = 0;
  count_names = argc - optind;
  while (optind < argc) {
    file = fopen(argv[optind], "r");
    if (file) {
      int flag_end = 0;
      while ((count_read_symbols = getline(&line, &len_str, file)) != EOF) {
        status = regexec(regex, line, 0, NULL, 0);
        str_count++;
        if ((options->v_flag && status == REG_NOMATCH) ||
            (status == 0 && (options->v_flag == 0 || options->e_flag))) {
          if (count_names > 1 && options->h_flag == 0 && options->c_flag == 0 &&
              options->l_flag == 0) {
            printf("%s:", argv[optind]);
          }
          if (options->n_flag && options->c_flag == 0 && options->l_flag == 0) {
            printf("%d:%s", str_count, line);
            if (line[strlen(line) - 1] != '\n') {
              printf("\n");
            }
          } else if (options->c_flag) {
            str_count_c++;
          } else if (options->l_flag && flag_end == 0) {
            printf("%s\n", argv[optind]);
            flag_end++;
          } else if (options->l_flag == 0) {
            printf("%s", line);
            if (line[strlen(line) - 1] != '\n') {
              printf("\n");
            }
          }
        }
      }
      if (options->c_flag) {
        if (count_names > 1 && options->h_flag == 0) {
          printf("%s:%d\n", argv[optind], str_count_c);
        } else {
          printf("%d\n", str_count_c);
        }
        str_count_c = 0;
      }
      if (options->n_flag) {
        str_count = 0;
      }
    } else {
      if (options->s_flag == 0) {
        fprintf(stderr, "s21_grep: %s: No such file or directory\n",
                argv[optind]);
      }
    }
    optind++;
  }
  fclose(file);
  free(line);
  regfree(regex);
  exit(0);
}