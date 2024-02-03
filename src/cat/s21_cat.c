#include "s21_cat.h"

int main(int argc, char *argv[]) { print_file_non_flags(argc, argv); }

void cat_read_flags(int argc, char *argv[], opt *options) {
  int opt = 0;
  static struct option long_options[] = {{"number-nonblank", 0, 0, 'b'},
                                         {"number", 0, 0, 'n'},
                                         {"squeeze-blank", 0, 0, 's'},
                                         {0, 0, 0, 0}};  // конец списка опций
  while ((opt = getopt_long(argc, argv, "benstvTE", long_options, 0)) != -1) {
    switch (opt) {
      case 'b':
        options->b_flag = 1;
        break;
      case 'e':
        options->e_flag = 1;
        options->v_flag = 1;
        break;
      case 'n':
        options->n_flag = 1;
        break;
      case 's':
        options->s_flag = 1;
        break;
      case 't':
        options->t_flag = 1;
        options->v_flag = 1;
        break;
      case 'v':
        options->v_flag = 1;
        break;
      case 'T':
        options->t_flag = 1;
        break;
      case 'E':
        options->e_flag = 1;
        break;
      default:
        fprintf(stderr, "error\n");
    }
  }
}

void cat_read_and_print_file(int argc, char *argv[], opt *options) {
  int current_symbol = 0;
  while (optind < argc) {
    FILE *fp = fopen(argv[optind], "r");
    if (fp) {
      int str_count = 1;
      int counter = 1;
      while ((current_symbol = fgetc(fp)) != EOF) {
        if (current_symbol == '\n' && counter > 1 && options->s_flag) {
          continue;
        }
        if (options->b_flag && counter && current_symbol != '\n') {
          printf("%6d\t", str_count++);
          counter = 0;
        }
        if ((options->n_flag && counter) && !(options->b_flag)) {
          printf("%6d\t", str_count++);
        }
        if (options->e_flag && current_symbol == '\n') {
          printf("$");
        }
        if (options->t_flag && current_symbol == '\t') {
          printf("^I");
          counter = 0;
          continue;
        }
        if (options->v_flag) {
          if ((current_symbol < 9 || current_symbol > 10) &&
              current_symbol < 32) {
            printf("%c", '^');
            current_symbol += 64;
          }
          if (current_symbol > 127 && current_symbol < 160) {
            printf("%c%c%c", 'M', '-', '^');
            current_symbol += 192;  // "À".
          } else if (current_symbol == 127) {
            current_symbol = '?';
            printf("^");
          }
        }
        if (current_symbol == '\n') {
          counter++;
        } else {
          counter = 0;
        }
        putchar(current_symbol);
      }
      fclose(fp);
    } else {
      fprintf(stderr, "s21_cat: %s: No such file or directory\n", argv[optind]);
    }
    optind++;
  }
}

void print_file_non_flags(int argc, char *argv[]) {
  if (argc == 1) {
    char current_symbol = getchar();
    while (current_symbol != EOF) {
      printf("%c", current_symbol);
      current_symbol = getchar();
    }
  } else {
    cat_read_flags(argc, argv, &options);
    cat_read_and_print_file(argc, argv, &options);
  }
}
