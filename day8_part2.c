/*
 * Routine: Advent of Code--Day 8: Playground (Part 2)
 *
 * Author: DannyBimma
 *
 * Copyright (c) 2025 Technomancer Pirate Caption. All Rights Reserved.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOXES 1000
#define MAX_PAIRS 500000

// Create a structure for the junction boxes
typedef struct {
  int x, y, z;
} Box;

// Create a structure for connected pairs of junction boxes
typedef struct {
  int box1, box2;
  double distance;
} Pair;

// Declare initial data
Box boxes[MAX_BOXES];
Pair pairs[MAX_PAIRS];
int parent[MAX_BOXES];
int rank_arr[MAX_BOXES];
int num_boxes = 0;

// Prototypes
void init_union_find(int n);
int find(int x);
int union_sets(int x, int y);
int count_circuits();
double calc_distance(Box *a, Box *b);
int compare_pairs(const void *a, const void *b);

int main() {
  // Open the input file for reading
  FILE *fp = fopen("day8_input.txt", "r");
  if (fp == NULL) {
    printf("Error opening file\n");

    return 1;
  }

  // Read junction boxes
  while (fscanf(fp, "%d,%d,%d", &boxes[num_boxes].x, &boxes[num_boxes].y,
                &boxes[num_boxes].z) == 3) {
    num_boxes++;
  }

  fclose(fp);

  printf("Read %d junction boxes\n", num_boxes);

  // Calculate all pairwise distances between junction boxes
  int num_pairs = 0;

  for (int i = 0; i < num_boxes; i++) {
    for (int j = i + 1; j < num_boxes; j++) {
      pairs[num_pairs].box1 = i;
      pairs[num_pairs].box2 = j;
      pairs[num_pairs].distance = calc_distance(&boxes[i], &boxes[j]);

      num_pairs++;
    }
  }

  printf("Calculated %d pairs\n", num_pairs);

  // Sort pairs by distance
  qsort(pairs, num_pairs, sizeof(Pair), compare_pairs);

  // Init Union-Find
  init_union_find(num_boxes);

  // Connect pairs until all junction boxes are in one circuit
  int last_box1 = -1, last_box2 = -1;
  int connections = 0;

  for (int i = 0; i < num_pairs; i++) {
    // Try to union these two boxes
    if (union_sets(pairs[i].box1, pairs[i].box2)) {
      // A connection was made
      last_box1 = pairs[i].box1;
      last_box2 = pairs[i].box2;
      connections++;

      // Check if all boxes are now in one circuit
      if (count_circuits() == 1) {
        printf("All boxes connected after %d connections\n", connections);
        printf("Last connection: box %d (%d,%d,%d) to box %d (%d,%d,%d)\n",
               last_box1, boxes[last_box1].x, boxes[last_box1].y,
               boxes[last_box1].z, last_box2, boxes[last_box2].x,
               boxes[last_box2].y, boxes[last_box2].z);
        printf("Distance: %.2f\n", pairs[i].distance);

        long long result =
            (long long)boxes[last_box1].x * (long long)boxes[last_box2].x;
        printf("Product of X coordinates: %d * %d = %lld\n", boxes[last_box1].x,
               boxes[last_box2].x, result);

        break;
      }
    }
  }

  return 0;
}

// Union-Finding functions
void init_union_find(int n) {
  for (int i = 0; i < n; i++) {
    parent[i] = i;
    rank_arr[i] = 0;
  }
}

int find(int x) {
  if (parent[x] != x)
    parent[x] = find(parent[x]); // Path compression

  return parent[x];
}

// Returns 1 if union was performed, 0 if they were already in same set
int union_sets(int x, int y) {
  int root_x = find(x);
  int root_y = find(y);

  if (root_x == root_y)
    return 0; // Already in same circuit

  // Union by rank
  if (rank_arr[root_x] < rank_arr[root_y]) {
    parent[root_x] = root_y;
  } else if (rank_arr[root_x] > rank_arr[root_y]) {
    parent[root_y] = root_x;
  } else {
    parent[root_y] = root_x;

    rank_arr[root_x]++;
  }

  return 1; // Union was performed
}

// Count the number of distinct circuits
int count_circuits() {
  int circuits = 0;
  for (int i = 0; i < num_boxes; i++) {
    if (find(i) == i)
      circuits++;
  }
  return circuits;
}

// Calc Euclidean distance
double calc_distance(Box *a, Box *b) {
  long long dx = (long long)a->x - b->x;
  long long dy = (long long)a->y - b->y;
  long long dz = (long long)a->z - b->z;

  return sqrt(dx * dx + dy * dy + dz * dz);
}

// Comparison function for sorting pairs
int compare_pairs(const void *a, const void *b) {
  Pair *p1 = (Pair *)a;
  Pair *p2 = (Pair *)b;
  if (p1->distance < p2->distance)
    return -1;
  if (p1->distance > p2->distance)
    return 1;

  return 0;
}
