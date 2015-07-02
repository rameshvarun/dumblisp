#include "expression.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

struct expr *create_builtin(builtin func_ptr) {
  // Malloc a new expression object
  struct expr *e = malloc(sizeof(struct expr));
  memset(e, 0, sizeof(struct expr));

  e->type = BUILTIN_EXPR;
  e->data.func_ptr = func_ptr;

  return e;
}

struct expr *create_empty_list() {
  // Malloc a new expression object
  struct expr *e = malloc(sizeof(struct expr));
  memset(e, 0, sizeof(struct expr));
  e->type = LIST_EXPR;
  return e;
}

struct expr *eval(struct scope *scope, struct expr *e) {
  // If this expression is quoted, then evaling it returns
  // the expression, unquoted.
  if (e->quoted) {
    e->quoted = false;
    return e;
  }

  switch (e->type) {
  // String and Integer expressions evaluate to themselves.
  case STRING_EXPR:
  case INT_EXPR:
    return e;
  case SYMBOL_EXPR: {
    struct expr *value = scope_lookup(scope, e->data.string_value);
    if (!value) {
      fprintf(stderr, "Symbol %s not bound to any value\n", e->data.string_value);
      exit(1);
    }
    return value;
  case LIST_EXPR: {
    // Empty list evaualtes to empty list
    if (e->data.head == NULL)
      return create_empty_list();
    else {
      struct expr *head = eval(scope, e->data.head);
      if (head->type == BUILTIN_EXPR) {
        return head->data.func_ptr(scope, e->data.head->next);
      } else {
        abort();
      }
    }
  }
  }
  default:
    fprintf(stderr, "Unkown expression type.\n");
    exit(1);
    return NULL;
  }
}
