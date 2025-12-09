/*
 * Routine: Advent of Code--Day 9: Movie Theatre (Part 2)
 *
 * Author: DannyBimma
 *
 * Copyright (c) 2025 Technomancer Pirate Caption. All Rights Reserved.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_POINTS 1000

typedef struct {
  int x;
  int y;
} Point;

// Prototypes
bool is_on_segment(Point p1, Point p2, int px, int py);
bool is_on_edge(Point *points, int count, int px, int py);
bool is_inside_polygon(Point *points, int count, int px, int py);
bool is_red(Point *points, int count, int px, int py);
bool is_red_or_green(Point *points, int count, int px, int py);

int main(void) {
  // Init main variables
  FILE *fp;
  Point points[MAX_POINTS];
  int count = 0;
  long long max_area = 0;

  // Open file input puzzle for reading
  fp = fopen("day9_input.txt", "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: Could not open day9_input.txt\n");

    return 1;
  }

  while (count < MAX_POINTS &&
         fscanf(fp, "%d,%d", &points[count].x, &points[count].y) == 2) {
    count++;
  }

  fclose(fp);

  if (count == 0) {
    fprintf(stderr, "Error: No points read from input file\n");

    return 1;
  }

  printf("Read %d red tiles\n", count);

  // Check all pairs of red tiles
  for (int i = 0; i < count; i++) {
    if (i % 50 == 0)
      printf("Progress: checking rectangles with corner %d/%d...\n", i, count);

    for (int j = i + 1; j < count; j++) {
      int min_x = (points[i].x < points[j].x) ? points[i].x : points[j].x;
      int max_x = (points[i].x > points[j].x) ? points[i].x : points[j].x;
      int min_y = (points[i].y < points[j].y) ? points[i].y : points[j].y;
      int max_y = (points[i].y > points[j].y) ? points[i].y : points[j].y;

      // Skip obviously huge rectangles to save time
      long long width = max_x - min_x + 1;
      long long height = max_y - min_y + 1;

      if (width > 10000 || height > 10000)
        continue;

      // Check if all tiles in the rectangle are red or green
      bool valid = true;

      for (int x = min_x; x <= max_x && valid; x++) {
        for (int y = min_y; y <= max_y && valid; y++) {
          if (!is_red_or_green(points, count, x, y))
            valid = false;
        }
      }

      if (valid) {
        long long area = width * height;

        if (area > max_area) {
          max_area = area;

          printf(
              "Found larger valid rectangle: corners at (%d,%d) and (%d,%d), "
              "area = %lld\n",
              points[i].x, points[i].y, points[j].x, points[j].y, area);
        }
      }
    }
  }

  printf("\nPart 2: Largest rectangle area (red/green only) = %lld\n",
         max_area);

  return 0;
}

// Check if point (px, py) is on the line segment from p1 to p2
bool is_on_segment(Point p1, Point p2, int px, int py) {
  if (p1.x == p2.x) {
    // Vertical segment
    int min_y = (p1.y < p2.y) ? p1.y : p2.y;
    int max_y = (p1.y > p2.y) ? p1.y : p2.y;

    return (px == p1.x && py >= min_y && py <= max_y);
  } else if (p1.y == p2.y) {
    //* Horizontal segment
    int min_x = (p1.x < p2.x) ? p1.x : p2.x;
    int max_x = (p1.x > p2.x) ? p1.x : p2.x;

    return (py == p1.y && px >= min_x && px <= max_x);
  }

  return false;
}

// Check if point (px, py) is on any edge connecting consecutive red tiles
bool is_on_edge(Point *points, int count, int px, int py) {
  for (int i = 0; i < count; i++) {
    int next = (i + 1) % count;
    if (is_on_segment(points[i], points[next], px, py))
      return true;
  }

  return false;
}

// Check if point is inside polygon using ray casting algorithm
bool is_inside_polygon(Point *points, int count, int px, int py) {
  int crossings = 0;

  for (int i = 0; i < count; i++) {
    int next = (i + 1) % count;
    Point p1 = points[i];
    Point p2 = points[next];

    // Cast a ray from (px, py) to the right (positive x direction)
    if (p1.x == p2.x) {
      // Vertical edge
      int min_y = (p1.y < p2.y) ? p1.y : p2.y;
      int max_y = (p1.y > p2.y) ? p1.y : p2.y;

      // Count crossing if edge is to the right and ray passes through it
      if (p1.x > px && py >= min_y && py < max_y)
        crossings++;
    }
    // Horizontal edges don't affect horizontal ray
  }

  return (crossings % 2) == 1;
}

// Check if point (px, py) is a red tile
bool is_red(Point *points, int count, int px, int py) {
  for (int i = 0; i < count; i++) {
    if (points[i].x == px && points[i].y == py) {

      return true;
    }
  }

  return false;
}

// Check if point is red or green (on edge or inside polygon)
bool is_red_or_green(Point *points, int count, int px, int py) {
  if (is_red(points, count, px, py))
    return true;

  if (is_on_edge(points, count, px, py))
    return true;

  if (is_inside_polygon(points, count, px, py))
    return true;

  return false;
}
