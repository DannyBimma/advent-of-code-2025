/*
 * Routine: Advent of Code--Day 1: Secret Entrance
 *
 * Author: DannyBimma
 *
 * Copyright (c) 2025 Technomancer Pirate Caption. All Rights Reserved.
 */

#include <stdio.h>
#include <stdlib.h>

int main() {
  char line[5000];
  int position = 50;
  int count = 0;

  // Open and read file
  FILE *secretSauce = fopen("day_1_file.txt", "r");
  if (secretSauce == NULL) {
    fprintf(stderr, "Error: Could not open input file\n");

    return 1;
  }

  // Scan rotation instructions line by line
  while (fgets(line, sizeof(line), secretSauce)) {
    char direction;
    int distance;

    // Count how many times 0 is passed during this rotation
    if (sscanf(line, " %c%d", &direction, &distance) == 2) {
      int zeros_during_rotation = 0;

      // R: count how many times rotation goes from 99 to 0
      if (direction == 'R') {
        zeros_during_rotation = (position + distance) / 100;
      } else {
        // L: count how many times rotation goes from 0 to 99
        if (position > 0 && distance >= position) {
          zeros_during_rotation = (distance - position) / 100 + 1;
        } else if (position == 0) {
          zeros_during_rotation = distance / 100;
        }
      }

      count += zeros_during_rotation;

      // Apply rotation with modulo arithmetic for wrapping
      if (direction == 'L') {
        position = ((position - distance) % 100 + 100) % 100;
      } else if (direction == 'R') {
        position = (position + distance) % 100;
      }
    }
  }

  fclose(secretSauce);

  printf("Password: %d\n", count);

  return 0;
}
