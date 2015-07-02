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
  return NULL;
}

struct expr *builtin_sub(struct scope *scope, struct expr *arguments) {
  return NULL;
}

struct expr *builtin_mult(struct scope *scope, struct expr *arguments) {
  return NULL;
}

struct expr *builtin_div(struct scope *scope, struct expr *arguments) {
  return NULL;
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
