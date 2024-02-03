#ifndef C3_SIMPLEBASHUTILS_S21_CAT_H
#define C3_SIMPLEBASHUTILS_S21_CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct options {
  int b_flag;
  int e_flag;
  int n_flag;
  int s_flag;
  int t_flag;
  int v_flag;
} opt;

opt options = {0};

void print_file_non_flags(int argc, char *argv[]);
void cat_read_flags(int argc, char *argv[], opt *options);
void cat_read_and_print_file(int argc, char *argv[], opt *options);

#endif