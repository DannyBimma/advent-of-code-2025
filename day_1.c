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

    // Apply rotation with modulo arithmetic for wrapping
    // Double modulo for L rotation because C
    if (sscanf(line, " %c%d", &direction, &distance) == 2) {
      if (direction == 'L') {
        position = ((position - distance) % 100 + 100) % 100;
      } else if (direction == 'R') {
        position = (position + distance) % 100;
      }

      // Increment counter after each rotation
      if (position == 0)
        count++;
    }
  }

  fclose(secretSauce);

  printf("Password: %d\n", count);

  return 0;
}
