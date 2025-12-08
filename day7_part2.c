/*
 * Routine: Advent of Code--Day 7 Part 2: Laboratories
 *
 * Author: DannyBimma
 *
 * Copyright (c) 2025 Technomancer Pirate Caption. All Rights Reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char grid[1000][1000];
static int rows = 0;
static int cols = 0;
static long long memo[1000][1000]; // Memoization cache (-1 = not computed)

// Prototype
long long count_timelines(int col, int row);

int main() {
  FILE *fp = fopen("day7_input.txt", "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: Could not open input file\n");

    return 1;
  }

  // Read the grid
  while (fgets(grid[rows], sizeof(grid[rows]), fp)) {
    int len = strlen(grid[rows]);

    if (len > 0 && grid[rows][len - 1] == '\n') {
      grid[rows][len - 1] = '\0';

      len--;
    }
    if (len > cols)
      cols = len;

    rows++;
  }

  fclose(fp);

  // Init memoization cache
  memset(memo, -1, sizeof(memo));

  // Find starting position S
  int start_col = -1, start_row = -1;
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < strlen(grid[r]); c++) {
      if (grid[r][c] == 'S') {
        start_col = c;
        start_row = r;

        break;
      }
    }
    if (start_col != -1)
      break;
  }

  // Count all timelines starting from S
  long long timeline_count = count_timelines(start_col, start_row);

  printf("Total timelines: %lld\n", timeline_count);

  return 0;
}

// Recursively count timelines from a given position
long long count_timelines(int col, int row) {
  // Check already computed
  if (memo[row][col] != -1)
    return memo[row][col];

  // Move particle downward
  int c = col;
  int r = row + 1;

  long long result;

  while (r < rows) {
    // Check if particle exits grid (left or right)
    if (c < 0 || c >= cols) {
      result = 1; // Exited - one timeline completed
      memo[row][col] = result;

      return result;
    }

    // Check for splitter
    if (grid[r][c] == '^') {
      long long total_timelines = 0;

      // Split into left timeline
      total_timelines += count_timelines(c - 1, r);

      // Split into right timeline
      total_timelines += count_timelines(c + 1, r);

      memo[row][col] = total_timelines;

      return total_timelines;
    }

    r++; // Continue moving down
  }

  // Exited bottom of grid - one timeline completed
  result = 1;
  memo[row][col] = result;

  return result;
}
