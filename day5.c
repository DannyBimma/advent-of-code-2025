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
int range_compare(const void *a, const void *b);

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

  fclose(fp);

  // Sort ranges by start point
  qsort(ranges, num_ranges, sizeof(struct Range), range_compare);

  // Merge overlapping ranges
  struct Range merged[1000];
  int num_merged = 0;

  merged[0] = ranges[0];
  num_merged = 1;

  for (int i = 1; i < num_ranges; i++) {
    struct Range *last = &merged[num_merged - 1];
    struct Range *current = &ranges[i];

    // Check if current overlaps with last merged range
    if (current->start <= last->end + 1) {
      // Merge: extend the end if needed
      if (current->end > last->end) {
        last->end = current->end;
      }
    } else {
      // No overlap = another merged range
      merged[num_merged] = *current;
      num_merged++;
    }
  }

  // Count total IDs in merged ranges
  long long total_count = 0;

  for (int i = 0; i < num_merged; i++) {
    long long count = merged[i].end - merged[i].start + 1;

    total_count += count;
  }

  printf("Total fresh ingredient IDs: %lld\n", total_count);

  return 0;
}

// Comparison function for qsort
int range_compare(const void *a, const void *b) {
  struct Range *r1 = (struct Range *)a;
  struct Range *r2 = (struct Range *)b;

  if (r1->start < r2->start)
    return -1;
  if (r1->start > r2->start)
    return 1;

  return 0;
}
