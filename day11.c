/*
 * Routine: Advent of Code--Day 11: Reactor
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
int count_paths(int current_idx, const char *target, bool *visited);

int main(void) {
  // Parse the input file
  parse_input("day11_input.txt");

  printf("Devices parsed: %d\n", device_count);

  // Find the starting device
  int start_idx = find_device("you");
  if (start_idx == -1) {
    fprintf(stderr, "Error: Starting device 'you' not found\n");

    return 1;
  }

  // Init visited array
  bool *visited = calloc(device_count, sizeof(bool));
  if (!visited) {
    fprintf(stderr, "Error: Memory allocation failed\n");

    return 1;
  }

  // Count all paths from "you" to "out"
  int path_count = count_paths(start_idx, "out", visited);

  printf("\nNumber of paths from 'you' to 'out': %d\n", path_count);

  free(visited);

  return 0;
}

// Find device index by name and return -1 upon failure
int find_device(const char *name) {
  for (int i = 0; i < device_count; i++) {
    if (strcmp(devices[i].name, name) == 0)
      return i;
  }
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

// DFS to count all paths from current device to target device
int count_paths(int current_idx, const char *target, bool *visited) {
  // If target reached, path found
  if (strcmp(devices[current_idx].name, target) == 0)
    return 1;

  // Mark current device as visited
  visited[current_idx] = true;

  int total_paths = 0;

  // Search all outputs
  for (int i = 0; i < devices[current_idx].output_count; i++) {
    int next_idx = find_device(devices[current_idx].outputs[i]);

    if (next_idx != -1 && !visited[next_idx])
      total_paths += count_paths(next_idx, target, visited);
  }

  // Backtrack: unmark current device
  visited[current_idx] = false;

  return total_paths;
}
