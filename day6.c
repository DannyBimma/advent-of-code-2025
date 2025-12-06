/*
 * Routine: Advent of Code--Day 6: Trash Compactor
 *
 * Author: DannyBimma
 *
 * Copyright (c) 2025 Technomancer Pirate Caption. All Rights Reserved.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  FILE *fp = fopen("day6_input.txt", "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: Could not open input file\n");

    return 1;
  }

  // Read all lines
  char lines[10][5000];
  int num_lines = 0;
  int max_len = 0;

  while (fgets(lines[num_lines], sizeof(lines[num_lines]), fp)) {
    int len = strlen(lines[num_lines]);

    if (len > 0 && lines[num_lines][len - 1] == '\n') {
      lines[num_lines][len - 1] = '\0';

      len--;
    }
    if (len > max_len)
      max_len = len;

    num_lines++;
  }

  fclose(fp);

  // Identify separator columns (the spaces)
  int is_separator[5000] = {0};

  for (int c = 0; c < max_len; c++) {
    int all_spaces = 1;

    for (int r = 0; r < num_lines; r++) {
      if (c < strlen(lines[r]) && lines[r][c] != ' ') {
        all_spaces = 0;

        break;
      }
    }
    is_separator[c] = all_spaces;
  }

  // Do the operations for each column
  long long grand_total = 0;
  int start = -1;

  for (int c = 0; c <= max_len; c++) {
    if (c == max_len || is_separator[c]) {
      if (start != -1) {
        // Found a problem from column start to c-1
        long long numbers[100];
        int num_count = 0;
        char operation = 0;

        for (int r = 0; r < num_lines; r++) {
          char buffer[100] = {0};
          int buf_idx = 0;

          // Extract chars from columns [start, c)
          for (int col = start; col < c; col++) {
            if (col < strlen(lines[r])) {
              buffer[buf_idx++] = lines[r][col];
            } else {
              buffer[buf_idx++] = ' ';
            }
          }
          buffer[buf_idx] = '\0';

          // Trim spaces
          char *ptr = buffer;

          while (*ptr == ' ')
            ptr++;

          int len = strlen(ptr);

          while (len > 0 && ptr[len - 1] == ' ')
            ptr[--len] = '\0';

          if (strlen(ptr) > 0) {
            if (r == num_lines - 1) {
              // Last row (the operator)
              operation = ptr[0];
            } else {
              // Number
              numbers[num_count++] = atoll(ptr);
            }
          }
        }

        // Calc result
        long long result;

        if (operation == '*') {
          result = 1;
          for (int i = 0; i < num_count; i++)
            result *= numbers[i];
        } else { // '+'
          result = 0;
          for (int i = 0; i < num_count; i++)
            result += numbers[i];
        }

        grand_total += result;
        start = -1;
      }
    } else {
      if (start == -1)
        start = c;
    }
  }

  printf("Grand total: %lld\n", grand_total);

  return 0;
}
