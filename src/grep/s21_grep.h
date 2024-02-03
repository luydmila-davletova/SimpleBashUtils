#ifndef C3_SIMPLEBASHUTILS_S21_GREP_H
#define C3_SIMPLEBASHUTILS_S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct options {
  int e_flag;
  int i_flag;
  int v_flag;
  int c_flag;
  int l_flag;
  int n_flag;
  int h_flag;
  int s_flag;
} opt;
opt options = {0};

void grep_read_flags(int argc, char *argv[], opt *options, regex_t *regex);
void grep_read_and_print_flags(int argc, char *argv[], opt *options,
                               regex_t *regex);
void print_file_non_flags(int argc, char *argv[]);

#endif  // C3_SIMPLEBASHUTILS_1_S21_GREP_H
