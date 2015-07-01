#include "scope.h"
#include "stddef.h"
#include "stdlib.h"
#include "string.h"

struct expr *scope_lookup(struct scope *scope, const char *symbol) {
  for (struct scope *curr = scope; curr != NULL; curr = curr->parent) {
    struct expr *e = scope_probe(curr, symbol);
    if (e)
      return e;
  }

  return NULL;
}

struct expr *scope_probe(struct scope *scope, const char *symbol) {
  for (struct mapping *curr = scope->mappings; curr != NULL; curr = curr->next) {
    if (strcmp(curr->symbol, symbol) == 0)
      return curr->value;
  }
  return NULL;
}

struct scope *scope_create(struct scope *parent) {
  struct scope *scope = malloc(sizeof(struct scope));
  scope->parent = parent;
  scope->mappings = NULL;
  return scope;
}

void scope_add_mapping(struct scope *scope, const char *symbol, struct expr *value) {
  struct mapping *m = malloc(sizeof(struct mapping));
  m->symbol = symbol;
  m->value = value;

  // Add this to the linked list of mappings
  m->next = scope->mappings;
  scope->mappings = m;
}
