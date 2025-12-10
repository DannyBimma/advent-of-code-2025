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
#include <string.h>

#define MAX_POINTS 1000
#define MAX_COORD 200000

typedef struct {
  int x;
  int y;
} Point;

// Prototypes
bool is_inside_or_on_polygon(Point *points, int count, int px, int py);
bool check_rectangle_valid(Point *points, int count, int min_x, int max_x,
                           int min_y, int max_y);

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

  printf("Tiles read: %d\n", count);

  // Check all pairs of red tiles
  for (int i = 0; i < count; i++) {
    if (i % 50 == 0)
      printf("Processing tiles: checking rectangles with corner: %d/%d...\n", i,
             count);

    for (int j = i + 1; j < count; j++) {
      int min_x = (points[i].x < points[j].x) ? points[i].x : points[j].x;
      int max_x = (points[i].x > points[j].x) ? points[i].x : points[j].x;
      int min_y = (points[i].y < points[j].y) ? points[i].y : points[j].y;
      int max_y = (points[i].y > points[j].y) ? points[i].y : points[j].y;

      long long width = max_x - min_x + 1;
      long long height = max_y - min_y + 1;
      long long area = width * height;

      // Skip if not potentially better than current max
      if (area <= max_area)
        continue;

      // Check if this rectangle is valid
      if (check_rectangle_valid(points, count, min_x, max_x, min_y, max_y)) {
        max_area = area;
        printf("Valid rectangle found: corners at (%d,%d) and (%d,%d), area: "
               "%lld\n",
               points[i].x, points[i].y, points[j].x, points[j].y, area);
      }
    }
  }

  printf("\nLargest rectangle area: (red/green only): %lld\n", max_area);

  return 0;
}

// Check if point is exactly on a red tile
bool is_red(Point *points, int count, int px, int py) {
  for (int i = 0; i < count; i++) {
    if (points[i].x == px && points[i].y == py)
      return true;
  }

  return false;
}

// Check if point is on the edge between two consecutive red tiles
bool is_on_edge(Point *points, int count, int px, int py) {
  for (int i = 0; i < count; i++) {
    int next = (i + 1) % count;
    Point p1 = points[i];
    Point p2 = points[next];

    // Check if point is on the horizontal or vertical line segment
    if (p1.x == p2.x && px == p1.x) {
      // Vertical segment
      int min_y = (p1.y < p2.y) ? p1.y : p2.y;
      int max_y = (p1.y > p2.y) ? p1.y : p2.y;

      if (py >= min_y && py <= max_y)
        return true;
    } else if (p1.y == p2.y && py == p1.y) {
      // Horizontal segment
      int min_x = (p1.x < p2.x) ? p1.x : p2.x;
      int max_x = (p1.x > p2.x) ? p1.x : p2.x;

      if (px >= min_x && px <= max_x)
        return true;
    }
  }

  return false;
}

// Check if point is inside the polygon using ray casting
// Use ray casting algorithm - cast ray to the right
bool is_inside_polygon(Point *points, int count, int px, int py) {
  int crossings = 0;

  for (int i = 0; i < count; i++) {
    int next = (i + 1) % count;
    Point p1 = points[i];
    Point p2 = points[next];

    // Only vertical edges can cross a horizontal ray
    if (p1.x == p2.x) {
      int edge_x = p1.x;
      int min_y = (p1.y < p2.y) ? p1.y : p2.y;
      int max_y = (p1.y > p2.y) ? p1.y : p2.y;

      // Check if ray crosses this edge
      // Edge must be to the right, and y must be in range
      if (edge_x > px && py >= min_y && py < max_y)
        crossings++;
    }
  }

  return (crossings % 2) == 1;
}

// Check if point is inside or on the polygon boundary
bool is_inside_or_on_polygon(Point *points, int count, int px, int py) {
  if (is_red(points, count, px, py))
    return true;

  if (is_on_edge(points, count, px, py))
    return true;

  if (is_inside_polygon(points, count, px, py))
    return true;

  return false;
}

// Optimised check for rectangle validity
bool check_rectangle_valid(Point *points, int count, int min_x, int max_x,
                           int min_y, int max_y) {
  // First, check all four corners - quick rejection test
  if (!is_inside_or_on_polygon(points, count, min_x, min_y))
    return false;

  if (!is_inside_or_on_polygon(points, count, max_x, min_y))
    return false;

  if (!is_inside_or_on_polygon(points, count, min_x, max_y))
    return false;

  if (!is_inside_or_on_polygon(points, count, max_x, max_y))
    return false;

  // Check edges of rectangle with sampling for large rectangles
  int width = max_x - min_x + 1;
  int height = max_y - min_y + 1;

  // For small rectangles, check every point
  if (width <= 100 && height <= 100) {
    for (int x = min_x; x <= max_x; x++) {
      for (int y = min_y; y <= max_y; y++) {
        if (!is_inside_or_on_polygon(points, count, x, y))
          return false;
      }
    }

    return true;
  }

  // For larger rectangles, sample a grid
  int step = (width > height) ? (width / 50) : (height / 50);

  if (step < 1)
    step = 1;

  for (int x = min_x; x <= max_x; x += step) {
    for (int y = min_y; y <= max_y; y += step) {
      if (!is_inside_or_on_polygon(points, count, x, y))
        return false;
    }
  }

  // Carefully check the boundaries
  for (int x = min_x; x <= max_x; x++) {
    if (!is_inside_or_on_polygon(points, count, x, min_y))
      return false;

    if (!is_inside_or_on_polygon(points, count, x, max_y))
      return false;
  }

  for (int y = min_y; y <= max_y; y++) {
    if (!is_inside_or_on_polygon(points, count, min_x, y))
      return false;

    if (!is_inside_or_on_polygon(points, count, max_x, y))
      return false;
  }

  return true;
}
