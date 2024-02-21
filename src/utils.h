#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Error utility macro
#define ON_ERROR_EXIT(cond, message)                                           \
  do {                                                                         \
    if ((cond)) {                                                              \
      printf("Error in function: %s at line %d\n", __func__, __LINE__);        \
      perror((message));                                                       \
      exit(1);                                                                 \
    }                                                                          \
  } while (0)

// returns how many paths survived the purge
static int remove_invalid_paths(char **path, int length) {
  for (int i = 0; i < length; i++) {
    if (access(path[i], F_OK) != 0) {
      length--;
      for (int j = i; j < length; j++) {
        strcpy(path[j], path[j + 1]);
      }
    }
  }
  return length;
}

// Check if a string "str" ends with a substring "ends"
static inline bool str_ends_in(const char *str, const char *ends) {
  const char *pos = strrchr(str, '.');
  return !strcmp(pos, ends);
}
