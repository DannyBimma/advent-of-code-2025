/*
 * Routine: Advent of Code--Day 2: Gift Shop
 *
 * Author: DannyBimma
 *
 * Copyright (c) 2025 Technomancer Pirate Caption. All Rights Reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Prototypes
int is_invalid_id(long long id);

int main() {
  long long total_sum = 0;

  FILE *fp = fopen("day2_input.txt", "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: Could not open input file\n");

    return 1;
  }

  // Read the entire line (it's one line, and it's long as fuck)
  char line[1000000]; // Big glizzy buffer

  if (fgets(line, sizeof(line), fp) == NULL) {
    fprintf(stderr, "Error: Could not read input\n");

    fclose(fp);

    return 1;
  }

  fclose(fp);

  // Parse each range separated by comma
  char *token = strtok(line, ",");

  while (token != NULL) {
    long long start, end;

    if (sscanf(token, "%lld-%lld", &start, &end) == 2) {
      // Check each ID in the range for validity
      for (long long id = start; id <= end; id++) {
        if (is_invalid_id(id))
          total_sum += id;
      }
    }

    token = strtok(NULL, ",");
  }

  printf("Sum of invalid IDs: %lld\n", total_sum);

  return 0;
}

int is_invalid_id(long long id) {
  // Create char array for id, convert id to string and get id length
  char str[50];
  sprintf(str, "%lld", id);
  int len = strlen(str);

  // Since pattern is repeated exactly twice, string length MUST be even
  if (len % 2 != 0)
    return 0;

  // Split in de middle
  int half = len / 2;

  // Compare first half with second half
  for (int i = 0; i < half; i++) {
    if (str[i] != str[i + half])
      return 0;
  }

  return 1;
}
