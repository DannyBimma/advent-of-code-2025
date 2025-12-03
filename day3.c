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
int find_max_joltage(char *line);

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
    int max_joltage = find_max_joltage(line);

    total_sum += max_joltage;
  }

  fclose(fp);

  printf("Total output joltage: %lld\n", total_sum);

  return 0;
}

int find_max_joltage(char *line) {
  int max_joltage = 0;
  int len = strlen(line);

  // Check for newline and rm if present
  if (len > 0 && line[len - 1] == '\n') {
    line[len - 1] = '\0';

    len--;
  }

  // Try all pairs of positions (i, j) where i < j
  for (int i = 0; i < len - 1; i++) {
    for (int j = i + 1; j < len; j++) {
      // Form a two-digit number from the pairs at positions i and j
      int joltage = (line[i] - '0') * 10 + (line[j] - '0');

      if (joltage > max_joltage)
        max_joltage = joltage;
    }
  }

  return max_joltage;
}
