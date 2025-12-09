/*
 * Routine: Advent of Code--Day 8: Playground
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
void union_sets(int x, int y);
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

  // Connect the 1000 closest pairs
  for (int i = 0; i < 1000; i++)
    union_sets(pairs[i].box1, pairs[i].box2);

  // Count circuit sizes
  int circuit_size[MAX_BOXES] = {0};
  for (int i = 0; i < num_boxes; i++) {
    int root = find(i);

    circuit_size[root]++;
  }

  // Find the 3 largest circuits
  int largest[3] = {0, 0, 0};

  for (int i = 0; i < num_boxes; i++) {
    if (circuit_size[i] > largest[0]) {
      largest[2] = largest[1];
      largest[1] = largest[0];
      largest[0] = circuit_size[i];
    } else if (circuit_size[i] > largest[1]) {
      largest[2] = largest[1];
      largest[1] = circuit_size[i];
    } else if (circuit_size[i] > largest[2]) {
      largest[2] = circuit_size[i];
    }
  }

  printf("Three largest circuits: %d, %d, %d\n", largest[0], largest[1],
         largest[2]);
  printf("Product: %d\n", largest[0] * largest[1] * largest[2]);

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

void union_sets(int x, int y) {
  int root_x = find(x);
  int root_y = find(y);

  if (root_x == root_y)
    return;

  // Union by rank
  if (rank_arr[root_x] < rank_arr[root_y]) {
    parent[root_x] = root_y;
  } else if (rank_arr[root_x] > rank_arr[root_y]) {
    parent[root_y] = root_x;
  } else {
    parent[root_y] = root_x;

    rank_arr[root_x]++;
  }
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
