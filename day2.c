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
  // Create char array for id, convert id to string, and get id length
  char str[50];
  sprintf(str, "%lld", id);
  int len = strlen(str);

  // Try all possible pattern lengths from 1 to len/2
  // Pattern MUST repeat at least twice: max pattern length is len/2
  for (int pattern_len = 1; pattern_len <= len / 2; pattern_len++) {
    // Pattern length MUST divide total length evenly
    if (len % pattern_len != 0)
      continue;

    // Check if the entire string is made of the repeating pattern
    int is_repeated = 1;

    for (int i = pattern_len; i < len; i++) {
      if (str[i] != str[i % pattern_len]) {
        is_repeated = 0;
        break;
      }
    }

    // If valid repetition, invalid identification
    if (is_repeated)
      return 1;
  }

  // No repeating pattern found
  return 0;
}
