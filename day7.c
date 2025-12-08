/*
 * Routine: Advent of Code--Day 7: Laboratories
 *
 * Author: DannyBimma
 *
 * Copyright (c) 2025 Technomancer Pirate Caption. All Rights Reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BEAMS 100000

// Create a structure for the Tachyon Beam
struct Beam {
  int col;
  int row;
};

int main() {
  FILE *fp = fopen("day7_input.txt", "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: Could not open input file\n");

    return 1;
  }

  // Read the grid
  char grid[1000][1000];
  int rows = 0;
  int cols = 0;

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

  // Find the beam entry point: S
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

  // BFS to process beams
  struct Beam queue[MAX_BEAMS];
  int head = 0, tail = 0;

  // Keep track of initial beams that were already processed
  int visited[1000][1000] = {0};

  // Start with initial beam at S
  queue[tail].col = start_col;
  queue[tail].row = start_row;

  tail++;

  visited[start_row][start_col] = 1;

  int split_count = 0;

  while (head < tail) {
    struct Beam beam = queue[head++];

    // Move beam downward until it hits a splitter or exits
    int c = beam.col;
    int r = beam.row + 1; // Start from next row

    while (r < rows) {
      // Check if valid position; exit if invalid
      if (c < 0 || c >= strlen(grid[r]))
        break;
      // Splitter
      if (grid[r][c] == '^') {
        split_count++;

        // Add two new beams (left and right of splitter)
        if (c - 1 >= 0 && !visited[r][c - 1]) {
          queue[tail].col = c - 1;
          queue[tail].row = r;

          tail++;

          visited[r][c - 1] = 1;
        }
        if (c + 1 < cols && !visited[r][c + 1]) {
          queue[tail].col = c + 1;
          queue[tail].row = r;

          tail++;

          visited[r][c + 1] = 1;
        }

        break; // Beam stops at splitter
      }
      r++; // Continue moving down
    }
  }

  printf("Total splits: %d\n", split_count);

  return 0;
}
