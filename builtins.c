#include "builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* ARITHMATIC OPERATORS */

struct expr *builtin_plus(struct scope *scope, struct expr *arguments) {
  if (arguments == NULL) {
    fprintf(stderr, "+ must have at least one argument.\n");
    exit(1);
  }

  int current_value = 0;
  for (struct expr *e = arguments; e != NULL; e = e->next) {
    struct expr *value = eval(scope, e);
    assert(value->type == INT_EXPR);
    current_value += value->data.int_value;
  }

  return create_int_expression(current_value);
}

struct expr *builtin_sub(struct scope *scope, struct expr *arguments) {
  if (arguments == NULL || arguments->next == NULL) {
    fprintf(stderr, "- must have at least two arguments.\n");
    exit(1);
  }

  struct expr *first = eval(scope, arguments);
  assert(first->type == INT_EXPR);
  int current_value = first->data.int_value;
  for (struct expr *e = arguments->next; e != NULL; e = e->next) {
    struct expr *value = eval(scope, e);
    assert(value->type == INT_EXPR);
    current_value -= value->data.int_value;
  }

  return create_int_expression(current_value);
}

struct expr *builtin_mult(struct scope *scope, struct expr *arguments) {
  if (arguments == NULL) {
    fprintf(stderr, "* must have at least one argument.\n");
    exit(1);
  }

  int current_value = 1;
  for (struct expr *e = arguments; e != NULL; e = e->next) {
    struct expr *value = eval(scope, e);
    assert(value->type == INT_EXPR);
    current_value *= value->data.int_value;
  }

  return create_int_expression(current_value);
}

struct expr *builtin_div(struct scope *scope, struct expr *arguments) {
  if (arguments == NULL || arguments->next == NULL) {
    fprintf(stderr, "/ must have at least two arguments.\n");
    exit(1);
  }

  struct expr *first = eval(scope, arguments);
  assert(first->type == INT_EXPR);
  int current_value = first->data.int_value;

  for (struct expr *e = arguments->next; e != NULL; e = e->next) {
    struct expr *value = eval(scope, e);
    assert(value->type == INT_EXPR);
    current_value /= value->data.int_value;
  }

  return create_int_expression(current_value);
}

/* SYSTEM CALL WRAPPERS */

struct expr *builtin_print(struct scope *scope, struct expr *arguments) {
  if (arguments == NULL) {
    fprintf(stderr, "print must have at least one argument.\n");
    exit(1);
  }

  for (struct expr *e = arguments; e != NULL; e = e->next) {
    struct expr *value = eval(scope, e);
    switch (value->type) {
    case STRING_EXPR:
      printf("%s", value->data.string_value);
      break;
    case INT_EXPR:
      printf("%d", value->data.int_value);
      break;
    default:
      break;
    }

    if (e->next != NULL) {
      printf(" ");
    }
  }

  printf("\n");
  return create_empty_list();
}

struct expr *builtin_let(struct scope *scope, struct expr *arguments) {
  if (arguments == NULL) {
    fprintf(stderr, "let must have at least one argument.\n");
    exit(1);
  }

  struct scope *new_scope = scope_create(scope);
  assert(arguments->type == LIST_EXPR);

  for (struct expr *mapping = arguments->data.head; mapping != NULL; mapping = mapping->next) {
    assert(mapping->type == LIST_EXPR);

    // Must have two arguments
    assert(mapping->data.head != NULL);
    assert(mapping->data.head->next != NULL);

    struct expr *symb = mapping->data.head;
    assert(symb->type == SYMBOL_EXPR);
    struct expr *val = eval(new_scope, mapping->data.head->next);
    scope_add_mapping(new_scope, symb->data.string_value, val);

    // TODO: Ensure that a symbol doesn't repeat
  }

  struct expr *last_value = create_empty_list();
  for (struct expr *e = arguments->next; e != NULL; e = e->next) {
    last_value = eval(new_scope, e);
  }

  return last_value;
}

struct expr *builtin_exit(struct scope *scope, struct expr *arguments) {
  return NULL;
}

struct expr *builtin_strlen(struct scope *scope, struct expr *arguments) {
  return NULL;
}

struct expr *builtin_strcmp(struct scope *scope, struct expr *arguments) {
  return NULL;
}

struct expr *builtin_strcat(struct scope *scope, struct expr *arguments) {
  return NULL;
}
