/*
 * Routine: Advent of Code--Day 4: Printing Department
 *
 * Author: DannyBimma
 *
 * Copyright (c) 2025 Technomancer Pirate Caption. All Rights Reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  FILE *fp = fopen("day4_input.txt", "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: Could not open input file\n");

    return 1;
  }

  // Read the grid into 2D char array
  char grid[1000][1000];
  int rows = 0;

  while (fgets(grid[rows], sizeof(grid[rows]), fp)) {
    int len = strlen(grid[rows]);
    // Trim newline
    if (len > 0 && grid[rows][len - 1] == '\n') {
      grid[rows][len - 1] = '\0';

      len--;
    }

    // Trim trailing spaces
    while (len > 0 && grid[rows][len - 1] == ' ') {
      grid[rows][len - 1] = '\0';

      len--;
    }

    // Skip leading spaces by shifting the string
    char *start = grid[rows];
    while (*start == ' ')
      start++;

    if (start != grid[rows])
      memmove(grid[rows], start, strlen(start) + 1);

    rows++;
  }

  fclose(fp);

  // 8 directions(all adjacent): N, NE, E, SE, S, SW, W, NW
  int dr[] = {-1, -1, 0, 1, 1, 1, 0, -1}; // Row
  int dc[] = {0, 1, 1, 1, 0, -1, -1, -1}; // Column

  int total_removed = 0;
  int current_removed;

  do {
    // Find all accessible rolls (< 4 neighbors) in current state
    char to_remove[1000][1000] = {0};
    current_removed = 0;

    for (int r = 0; r < rows; r++) {
      int cols = strlen(grid[r]);
      for (int c = 0; c < cols; c++) {
        if (grid[r][c] == '@') {
          // Count adjacent '@' symbols
          int neighbor_count = 0;

          for (int d = 0; d < 8; d++) {
            int nr = r + dr[d];
            int nc = c + dc[d];

            // Bounds check
            if (nr >= 0 && nr < rows && nc >= 0 && nc < strlen(grid[nr])) {
              if (grid[nr][nc] == '@')
                neighbor_count++;
            }
          }

          if (neighbor_count < 4) {
            to_remove[r][c] = 1;

            current_removed++;
          }
        }
      }
    }

    // Remove all marked rolls at once
    for (int r = 0; r < rows; r++) {
      int cols = strlen(grid[r]);
      for (int c = 0; c < cols; c++) {
        if (to_remove[r][c])
          grid[r][c] = '.';
      }
    }

    total_removed += current_removed;

  } while (current_removed > 0);

  printf("Total removed: %d\n", total_removed);

  return 0;
}
