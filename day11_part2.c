/*
 * Routine: Advent of Code--Day 11: Reactor (Part 2)
 *
 * Author: DannyBimma
 *
 * Copyright (c) 2025 Technomancer Pirate Caption. All Rights Reserved.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DEVICES 1000
#define MAX_NAME_LEN 10
#define MAX_OUTPUTS 50
#define MAX_LINE_LEN 512

// Create a structure to represent a device and its outputs
typedef struct {
  char name[MAX_NAME_LEN];
  char outputs[MAX_OUTPUTS][MAX_NAME_LEN];
  int output_count;
} Device;

// Put graph data in global scope
Device devices[MAX_DEVICES];
int device_count = 0;

// Prototypes
int find_device(const char *name);
int add_device(const char *name);
void parse_input(const char *filename);
void init_memo();
long long count_paths_with_required(int current_idx, const char *target,
                                    bool *visited, bool seen_dac,
                                    bool seen_fft);

int main(void) {
  // Parse the input file
  parse_input("day11_input.txt");

  printf("Devices parsed: %d\n", device_count);

  // Find the starting device "svr"
  int start_idx = find_device("svr");

  if (start_idx == -1) {
    fprintf(stderr, "Error: Starting device 'svr' not found\n");
    return 1;
  }

  // Init visited array
  bool *visited = calloc(device_count, sizeof(bool));
  if (!visited) {
    fprintf(stderr, "Error: Memory allocation failed\n");
    return 1;
  }

  // Init memoization table
  init_memo();

  // Count paths from "svr" to "out" that visit both "dac" and "fft"
  long long path_count =
      count_paths_with_required(start_idx, "out", visited, false, false);

  printf("\nNumber of paths from 'svr' to 'out': visiting both 'dac' and "
         "'fft': %lld\n",
         path_count);

  free(visited);

  return 0;
}

// Find device index by name and return -1 upon failure
int find_device(const char *name) {
  for (int i = 0; i < device_count; i++)
    if (strcmp(devices[i].name, name) == 0)
      return i;

  return -1;
}

// Add a device to the graph
// Return its index if it doesn't exist
int add_device(const char *name) {
  int idx = find_device(name);

  if (idx == -1) {
    strncpy(devices[device_count].name, name, MAX_NAME_LEN - 1);

    devices[device_count].name[MAX_NAME_LEN - 1] = '\0';
    devices[device_count].output_count = 0;
    idx = device_count;

    device_count++;
  }
  return idx;
}

// Open input file, read it, and build the graph
void parse_input(const char *filename) {
  FILE *fp = fopen(filename, "r");

  if (!fp) {
    fprintf(stderr, "Error: Cannot open file %s\n", filename);
    exit(1);
  }

  char line[MAX_LINE_LEN];
  while (fgets(line, sizeof(line), fp)) {
    // Remove newline
    line[strcspn(line, "\n")] = '\0';

    // Parse device name (string before colon)
    char *colon = strchr(line, ':');
    if (!colon)
      continue;

    *colon = '\0';
    char *device_name = line;

    int device_idx = add_device(device_name);

    // Parse outputs (string after colon)
    char *outputs_str = colon + 1;
    char *token = strtok(outputs_str, " \t");

    while (token != NULL) {
      // Skip empty tokens
      if (strlen(token) > 0) {
        // Add this device as an output
        int out_count = devices[device_idx].output_count;
        strncpy(devices[device_idx].outputs[out_count], token,
                MAX_NAME_LEN - 1);
        devices[device_idx].outputs[out_count][MAX_NAME_LEN - 1] = '\0';
        devices[device_idx].output_count++;

        // Ensure the output device exists in the list
        add_device(token);
      }
      token = strtok(NULL, " \t");
    }
  }

  fclose(fp);
}

// Memoization table: memo[node_idx][seen_dac][seen_fft] = path count
// -1 means not yet computed
static long long memo[MAX_DEVICES][2][2];
static bool memo_initialized = false;

void init_memo() {
  if (!memo_initialized) {
    for (int i = 0; i < MAX_DEVICES; i++) {
      for (int j = 0; j < 2; j++) {
        for (int k = 0; k < 2; k++) {
          memo[i][j][k] = -1;
        }
      }
    }
    memo_initialized = true;
  }
}

// DFS to count paths that visit both required nodes with memoization
// Tracks whether "dac" and "fft" have been visited in the current path
long long count_paths_with_required(int current_idx, const char *target,
                                    bool *visited, bool seen_dac,
                                    bool seen_fft) {
  // Update if required nodes have been seen
  if (strcmp(devices[current_idx].name, "dac") == 0)
    seen_dac = true;
  if (strcmp(devices[current_idx].name, "fft") == 0)
    seen_fft = true;

  // If target reached, check if both required nodes were visited
  if (strcmp(devices[current_idx].name, target) == 0)
    return (seen_dac && seen_fft) ? 1 : 0;

  // Check memoization table (if not in visited path to avoid cycles)
  int dac_flag = seen_dac ? 1 : 0;
  int fft_flag = seen_fft ? 1 : 0;

  if (!visited[current_idx] && memo[current_idx][dac_flag][fft_flag] != -1)
    return memo[current_idx][dac_flag][fft_flag];

  // Mark current device as visited
  visited[current_idx] = true;

  long long total_paths = 0;

  // Search all outputs
  for (int i = 0; i < devices[current_idx].output_count; i++) {
    int next_idx = find_device(devices[current_idx].outputs[i]);

    if (next_idx != -1 && !visited[next_idx]) {
      total_paths += count_paths_with_required(next_idx, target, visited,
                                               seen_dac, seen_fft);
    }
  }

  // Backtrack: unmark current device
  visited[current_idx] = false;

  // Store in memo table
  memo[current_idx][dac_flag][fft_flag] = total_paths;

  return total_paths;
}
