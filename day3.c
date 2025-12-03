/*
 * Routine: Advent of Code--Day 3: Lobby
 *
 * Author: DannyBimma
 *
 * Copyright (c) 2025 Technomancer Pirate Caption. All Rights Reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Prototype
long long find_max_joltage(char *line);

int main() {
  FILE *fp = fopen("day3_input.txt", "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: Could not open input file\n");

    return 1;
  }

  char line[10000]; // Big buffer for each bank
  long long total_sum = 0;

  // Process each bank/line
  while (fgets(line, sizeof(line), fp)) {
    long long max_joltage = find_max_joltage(line);

    total_sum += max_joltage;
  }

  fclose(fp);

  printf("Total output joltage: %lld\n", total_sum);

  return 0;
}

long long find_max_joltage(char *line) {
  int len = strlen(line);

  // Check for newline and rm if present
  if (len > 0 && line[len - 1] == '\n') {
    line[len - 1] = '\0';

    len--;
  }

  // Greedy algo to select 12 digits that form the largest number
  int k = 12;      // Num of digit
  char result[13]; // Digits + null terminator
  int result_idx = 0;
  int start = 0;
  int remaining = k;

  for (int i = 0; i < k; i++) {
    // Search: start + (n - start - remaining)
    // Leave enough digits for future picks
    int search_end = start + (len - start - remaining);

    // Find the largest digit from [start, search_end]
    char max_digit = '0';
    int max_pos = start;

    for (int j = start; j <= search_end; j++) {
      if (line[j] > max_digit) {
        max_digit = line[j];
        max_pos = j;
      }
    }

    // Add digit to result
    result[result_idx++] = max_digit;

    // Move to next position
    start = max_pos + 1;

    remaining--;
  }

  result[result_idx] = '\0';

  // Convert 12-digit string to a long long number
  long long joltage = strtoll(result, NULL, 10);

  return joltage;
}
