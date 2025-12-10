/*
 * Routine: Advent of Code--Day 10: Factory
 *
 * Author: DannyBimma
 *
 * Copyright (c) 2025 Technomancer Pirate Caption. All Rights Reserved.
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LIGHTS 16
#define MAX_BUTTONS 32
#define MAX_LINE 1024

// Create a structure to represent the machine
// With the state of lights and button effects
typedef struct {
  int num_lights;
  unsigned int target; // Bit pattern for target configuration
  int num_buttons;
  unsigned int buttons[MAX_BUTTONS]; // Each button's effect as bit pattern
} Machine;

int count_bits(unsigned int n);
int parse_machine(const char *line, Machine *machine);
int machine_diagnostic(Machine *machine);

int main(void) {
  // Open puzzle input file for reading
  FILE *fp = fopen("day10_input.txt", "r");
  if (!fp) {
    perror("Error opening file");

    return 1;
  }

  // Init main variables
  char line[MAX_LINE];
  int total_presses = 0;
  int machine_count = 0;

  while (fgets(line, sizeof(line), fp)) {
    // Skip empty lines
    if (line[0] == '\n' || line[0] == '\0')
      continue;

    Machine machine;
    if (parse_machine(line, &machine)) {
      int min_presses = machine_diagnostic(&machine);

      if (min_presses == INT_MAX) {
        printf("Machine %d: No solution found!\n", machine_count + 1);
      } else {
        total_presses += min_presses;
        machine_count++;
      }
    }
  }

  fclose(fp);

  printf("Total number of machines diagnosed: %d\n", machine_count);
  printf("Fewest button presses required: %d\n", total_presses);

  return 0;
}

// Count number of set bits (popcount)
int count_bits(unsigned int n) {
  int count = 0;

  while (n) {
    count += n & 1;
    n >>= 1;
  }

  return count;
}

// Parser for the machines/lines
int parse_machine(const char *line, Machine *machine) {
  const char *p = line;

  // Skip to '[' and parse target pattern
  while (*p && *p != '[')
    p++;

  if (*p != '[')
    return 0;
  p++;

  machine->num_lights = 0;
  machine->target = 0;

  while (*p && *p != ']') {
    if (*p == '#')
      machine->target |= (1 << machine->num_lights);

    machine->num_lights++;
    p++;
  }

  if (*p != ']')
    return 0;
  p++;

  // Parse buttons
  machine->num_buttons = 0;

  while (*p) {
    // Find next '('
    while (*p && *p != '(' && *p != '{')
      p++;

    if (*p == '{')
      break; // Reached joltage section

    if (*p != '(')
      break;
    p++;

    // Parse button definition
    unsigned int button = 0;

    while (*p && *p != ')') {
      if (*p >= '0' && *p <= '9') {
        int light_idx = 0;

        while (*p >= '0' && *p <= '9') {
          light_idx = light_idx * 10 + (*p - '0');

          p++;
        }
        button |= (1 << light_idx);
      } else {
        p++;
      }
    }

    if (*p == ')') {
      machine->buttons[machine->num_buttons++] = button;

      p++;
    }
  }

  return 1;
}

// Find least amount of button presses to activate a machine
int machine_diagnostic(Machine *machine) {
  int min_presses = INT_MAX;

  // Brute force all 2^num_buttons combinations
  unsigned int total_combinations = 1U << machine->num_buttons;

  for (unsigned int combo_special = 0; combo_special < total_combinations;
       combo_special++) {
    unsigned int state = 0; // Init all lights off

    // Apply each button press in the special combination
    for (int i = 0; i < machine->num_buttons; i++) {
      if (combo_special & (1U << i))
        // XOR toggle
        state ^= machine->buttons[i];
    }
    // Check if target reached
    if (state == machine->target) {
      int presses = count_bits(combo_special);

      if (presses < min_presses)
        min_presses = presses;
    }
  }

  return min_presses;
}
