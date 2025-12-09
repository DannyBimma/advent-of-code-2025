/*
 * Routine: Advent of Code--Day 9: Movie Theatre
 *
 * Author: DannyBimma
 *
 * Copyright (c) 2025 Technomancer Pirate Caption. All Rights Reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_POINTS 1000

typedef struct {
  int x;
  int y;
} Point;

int main(void) {
  // Init main variables
  FILE *fp;
  Point points[MAX_POINTS];
  int count = 0;
  long long max_area = 0;

  // Open puzzle input file
  fp = fopen("day9_input.txt", "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: Could not open day9_input.txt\n");

    return 1;
  }

  // Parse all coordinate pairs from input file
  while (count < MAX_POINTS &&
         fscanf(fp, "%d,%d", &points[count].x, &points[count].y) == 2) {
    count++;
  }

  fclose(fp);

  if (count == 0) {
    fprintf(stderr, "Error: No points read from input file\n");

    return 1;
  }

  printf("Tile coordinates read from input: %d\n", count);

  // Check all pairs of points to find largest rectangle
  for (int i = 0; i < count; i++) {
    for (int j = i + 1; j < count; j++) {
      int dx = points[j].x - points[i].x;
      int dy = points[j].y - points[i].y;

      // Calc area using absolute values to handle any ordering
      // +1 because coordinates represent tile positions, not boundaries
      long long width = (dx < 0) ? -dx : dx;
      long long height = (dy < 0) ? -dy : dy;
      width += 1;
      height += 1;
      long long area = width * height;

      if (area > max_area)
        max_area = area;
    }
  }

  printf("Largest rectangle area: %lld\n", max_area);

  return 0;
}
