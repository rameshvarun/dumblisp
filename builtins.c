#include "builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* ARITHMATIC OPERATORS */

struct expr *builtin_plus(struct scope *scope, struct expr *arguments) {
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

struct expr *builtin_puts(struct scope *scope, struct expr *arguments) {
  assert(arguments != NULL);
  struct expr *arg = eval(scope, arguments);
  assert(arguments->type == STRING_EXPR);

  puts(arg->data.string_value);
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
