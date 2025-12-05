/*
 * Routine: Advent of Code--Day 5: Cafeteria
 *
 * Author: DannyBimma
 *
 * Copyright (c) 2025 Technomancer Pirate Caption. All Rights Reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Create a simple start to end structure
struct Range {
  long long start;
  long long end;
};

// Prototype
int is_fresh(long long id, struct Range ranges[], int num_ranges);

int main() {
  FILE *fp = fopen("day5_input.txt", "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: Could not open input file\n");

    return 1;
  }

  // Read ranges until blank line
  struct Range ranges[1000];
  int num_ranges = 0;
  char line[256];

  while (fgets(line, sizeof(line), fp)) {
    // Check for blank line (separator)
    if (line[0] == '\n' || line[0] == '\r')
      break;
    // Get the ranges
    long long start, end;
    if (sscanf(line, "%lld-%lld", &start, &end) == 2) {
      ranges[num_ranges].start = start;
      ranges[num_ranges].end = end;

      num_ranges++;
    }
  }

  // Read available IDs and count the fresh ones
  int fresh_count = 0;
  long long id;

  while (fscanf(fp, "%lld", &id) == 1) {
    if (is_fresh(id, ranges, num_ranges))
      fresh_count++;
  }

  fclose(fp);

  printf("Fresh ingredients: %d\n", fresh_count);

  return 0;
}

// Check if ingredient id falls within fresh range
int is_fresh(long long id, struct Range ranges[], int num_ranges) {
  for (int i = 0; i < num_ranges; i++) {
    if (id >= ranges[i].start && id <= ranges[i].end)
      return 1;
  }

  return 0;
}
