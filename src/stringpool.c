#include "stringpool.h"
#include "defs.h"
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>

char *strings = NULL;

static inline const char *next(const char *string) {
  // Advance backwards by sizeof(char**) and dereference.
  char **next = (char **)string - 1;
  return *next;
}

const char *stringpool_add(const char *newstring) {
  for (const char *current = strings; current != NULL; current = next(current)) {
    if (strcmp(current, newstring) == 0) {
      return current;
    }
  }

  // Create a block which contains a next pointer and the string data.
  void *data = malloc(sizeof(char *) + strlen(newstring) + 1);

  // Copy in the string data.
  char *stringdata = (char *)((char **)data + 1);
  strcpy(stringdata, newstring);

  // Add this data block to the linked list.
  *(char **)data = strings;
  strings = stringdata;

  return stringdata;
}