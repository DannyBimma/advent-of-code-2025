/*
 * Routine: Advent of Code--Day 10 Part 2: Factory Joltage
 *
 * Author: DannyBimma
 *
 * Copyright (c) 2025 Technomancer Pirate Caption. All Rights Reserved.
 *
 * Solution using COIN-OR Branch and Cut (Cbc) Integer Linear Programming
 *
 * To compile: gcc -std=c99 -O2 day10_part2.c $(pkg-config --cflags --libs cbc)
 * -o day10_part2
 */

#include <Cbc_C_Interface.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COUNTERS 16
#define MAX_BUTTONS 32
#define MAX_LINE 1024

typedef struct {
  int num_counters;
  int targets[MAX_COUNTERS];
  int num_buttons;
  int button_effects[MAX_BUTTONS][MAX_COUNTERS];
} Machine;

// Prototypes
int parse_machine_part2(const char *line, Machine *machine);
int solve_machine_ilp(const Machine *machine, int machine_id);

int main(void) {
  FILE *fp = fopen("day10_input.txt", "r");
  if (!fp) {
    perror("Error opening file");

    return 1;
  }

  // Init main variables
  char line[MAX_LINE];
  int total_presses = 0;
  int machine_count = 0;
  int solved_count = 0;

  printf("Processing: Cbc Library--Integer Linear Programming...\n\n");

  while (fgets(line, sizeof(line), fp)) {
    if (line[0] == '\n' || line[0] == '\0')
      continue;

    machine_count++;
    Machine machine;

    if (!parse_machine_part2(line, &machine)) {
      printf("Machine %d: Failed to parse\n", machine_count);
      continue;
    }

    int result = solve_machine_ilp(&machine, machine_count);

    if (result >= 0) {
      total_presses += result;
      solved_count++;
      if (machine_count <= 10) {
        printf("Machine %d: %d presses ✓\n", machine_count, result);
      }
    } else {
      printf("Machine %d: No solution found ✗\n", machine_count);
    }
  }

  fclose(fp);

  printf("\n");
  printf("==================================================\n");
  printf("Total machines: %d\n", machine_count);
  printf("Solved: %d\n", solved_count);
  printf("Failed: %d\n", machine_count - solved_count);
  printf("Fewest button presses required: %d\n", total_presses);
  printf("==================================================\n");

  return 0;
}

int parse_machine_part2(const char *line, Machine *machine) {
  const char *p = line;

  // Skip indicator lights section [...]
  while (*p && *p != '[')
    p++;

  if (*p != '[')
    return 0;

  while (*p && *p != ']')
    p++;

  if (*p != ']')
    return 0;
  p++;

  // Init
  machine->num_buttons = 0;
  for (int i = 0; i < MAX_BUTTONS; i++) {
    for (int j = 0; j < MAX_COUNTERS; j++) {
      machine->button_effects[i][j] = 0;
    }
  }

  // Parse buttons
  int button_idx = 0;
  while (*p && *p != '{') {
    while (*p && *p != '(' && *p != '{')
      p++;

    if (*p == '{')
      break;

    if (*p != '(')
      break;
    p++;

    while (*p && *p != ')') {
      if (*p >= '0' && *p <= '9') {
        int counter_idx = 0;

        while (*p >= '0' && *p <= '9') {
          counter_idx = counter_idx * 10 + (*p - '0');

          p++;
        }
        machine->button_effects[button_idx][counter_idx] = 1;
      } else {
        p++;
      }
    }

    if (*p == ')') {
      button_idx++;
      p++;
    }
  }
  machine->num_buttons = button_idx;

  // Parse joltage requirements
  while (*p && *p != '{')
    p++;

  if (*p != '{')
    return 0;
  p++;

  int target_idx = 0;

  while (*p && *p != '}') {
    if (*p >= '0' && *p <= '9') {
      int value = 0;

      while (*p >= '0' && *p <= '9') {
        value = value * 10 + (*p - '0');

        p++;
      }
      machine->targets[target_idx++] = value;
    } else {
      p++;
    }
  }

  machine->num_counters = target_idx;

  return 1;
}

int solve_machine_ilp(const Machine *machine, int machine_id) {
  Cbc_Model *model = Cbc_newModel();

  // Suppress output
  Cbc_setLogLevel(model, 0);

  // Add columns (variables): one for each button
  // Variables are non-negative integers (number of button presses)
  for (int b = 0; b < machine->num_buttons; b++) {
    char var_name[32];
    snprintf(var_name, sizeof(var_name), "button_%d", b);

    // Parameters: name, lower_bound, upper_bound, objective_coef, isInteger,
    // nz, rows, coefs
    // Minimise button presses, objective coefficient is 1
    // Add constraints separately: nz=0, rows=NULL, coefs=NULL
    Cbc_addCol(model, var_name, 0.0, 1e10, 1.0, 1, 0, NULL, NULL);
  }

  // Add rows (constraints): one for each counter
  // Each constraint: sum(button_presses[b] * effect[b][c]) = target[c]
  for (int c = 0; c < machine->num_counters; c++) {
    // Count how many buttons affect this counter
    int nz = 0;

    for (int b = 0; b < machine->num_buttons; b++) {
      if (machine->button_effects[b][c] == 1)
        nz++;
    }

    if (nz == 0)
      // No buttons affect this counter (shouldn't happen 🤷🏽‍♂️)
      continue;

    // Build arrays of column indices and coefficients
    int *cols = malloc(nz * sizeof(int));
    double *coefs = malloc(nz * sizeof(double));

    int idx = 0;
    for (int b = 0; b < machine->num_buttons; b++) {
      if (machine->button_effects[b][c] == 1) {
        cols[idx] = b;
        coefs[idx] = 1.0;

        idx++;
      }
    }

    char constraint_name[32];
    snprintf(constraint_name, sizeof(constraint_name), "counter_%d", c);

    // Add constraint: sum = target (sense 'E' for equality)
    Cbc_addRow(model, constraint_name, nz, cols, coefs, 'E',
               (double)machine->targets[c]);

    free(cols);
    free(coefs);
  }

  // Solve the model
  Cbc_solve(model);

  // Check if solution found
  int status = Cbc_isProvenOptimal(model);

  int result = -1;
  if (status) {
    // Get solution
    const double *solution = Cbc_getColSolution(model);

    // Calc total button presses
    int total = 0;

    for (int b = 0; b < machine->num_buttons; b++)
      // Round to nearest integer
      total += (int)(solution[b] + 0.5);

    // Validate solution
    int valid = 1;

    for (int c = 0; c < machine->num_counters; c++) {
      int count = 0;

      for (int b = 0; b < machine->num_buttons; b++)
        count += ((int)(solution[b] + 0.5)) * machine->button_effects[b][c];

      if (count != machine->targets[c]) {
        valid = 0;

        break;
      }
    }

    if (valid) {
      result = total;
    }
  }

  // Clean up
  Cbc_deleteModel(model);

  return result;
}
