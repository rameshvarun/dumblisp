#include "builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* CONTROL FLOW */
struct expr *builtin_if(struct scope *scope, struct expr *arguments) {
  if (arguments == NULL || arguments->next == NULL) {
    fprintf(stderr, "if must have at least two arguments.\n");
    exit(1);
  }

  struct expr *cond = eval(scope, arguments);
  assert(cond->type = BOOL_EXPR);

  // If the condiiton evaluates to true
  if (cond->data.boolean_value) {
    return eval(scope, arguments->next);
  }

  // If the conditon evaluates to false
  if (arguments->next->next != NULL) {
    return eval(scope, arguments->next->next);
  }

  return create_empty_list();
}

struct expr *builtin_while(struct scope *scope, struct expr *arguments) {
  if (arguments == NULL || arguments->next == NULL) {
    fprintf(stderr, "if must have at least two arguments.\n");
    exit(1);
  }

  while (true) {
    // Evaulate the condition.
    struct expr *cond = eval(scope, arguments);
    assert(cond->type = BOOL_EXPR);

    // If the condiiton evaluates to false, exit loop.
    if (!cond->data.boolean_value)
      break;

    // Otherwise evaluate body statements.
    for (struct expr *e = arguments->next; e != NULL; e = e->next) {
      eval(scope, e);
    }
  }

  return create_empty_list();
}

struct expr *builtin_or(struct scope *scope, struct expr *arguments) {
  if (arguments == NULL || arguments->next == NULL) {
    fprintf(stderr, "= must have at least two arguments.");
  }
  struct expr *first = eval(scope, arguments);
  struct expr *second = eval(scope, arguments->next);
  assert(first->type == BOOL_EXPR);
  assert(second->type == BOOL_EXPR);
  return create_bool_expression(first->data.boolean_value || second->data.boolean_value);
}

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

struct expr *builtin_eq(struct scope *scope, struct expr *arguments) {
  if (arguments == NULL || arguments->next == NULL) {
    fprintf(stderr, "= must have at least two arguments.");
  }
  struct expr *first = eval(scope, arguments);
  struct expr *second = eval(scope, arguments->next);
  assert(first->type == INT_EXPR);
  assert(second->type == INT_EXPR);
  return create_bool_expression(first->data.int_value == second->data.int_value);
}

struct expr *builtin_lt(struct scope *scope, struct expr *arguments) {
  if (arguments == NULL || arguments->next == NULL) {
    fprintf(stderr, "= must have at least two arguments.");
  }
  struct expr *first = eval(scope, arguments);
  struct expr *second = eval(scope, arguments->next);
  assert(first->type == INT_EXPR);
  assert(second->type == INT_EXPR);
  return create_bool_expression(first->data.int_value < second->data.int_value);
}

struct expr *builtin_gt(struct scope *scope, struct expr *arguments) {
  if (arguments == NULL || arguments->next == NULL) {
    fprintf(stderr, "= must have at least two arguments.");
  }
  struct expr *first = eval(scope, arguments);
  struct expr *second = eval(scope, arguments->next);
  assert(first->type == INT_EXPR);
  assert(second->type == INT_EXPR);
  return create_bool_expression(first->data.int_value > second->data.int_value);
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
    case BOOL_EXPR:
      if (value->data.boolean_value)
        printf("TRUE");
      else
        printf("FALSE");
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

struct expr *builtin_plet(struct scope *scope, struct expr *arguments) {
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
    last_value = eval(scope, e);
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

struct expr *builtin_lambda(struct scope *scope, struct expr *arguments) {
  if (arguments == NULL || arguments->next == NULL) {
    fprintf(stderr, "lambda must have at least two arguments.\n");
    exit(1);
  }

  assert(arguments->type == LIST_EXPR);
  return create_func_expression(arguments->data.head, scope, arguments->next);
}

struct expr *builtin_defun(struct scope *scope, struct expr *arguments) {
  if (arguments == NULL || arguments->next == NULL || arguments->next->next == NULL) {
    fprintf(stderr, "defun must have at least three arguments.\n");
    exit(1);
  }

  assert(arguments->type == SYMBOL_EXPR);
  assert(arguments->next->type == LIST_EXPR);

  scope_add_mapping(
      scope, arguments->data.string_value,
      create_func_expression(arguments->next->data.head, scope, arguments->next->next));
  return create_empty_list();
}

struct expr *builtin_set(struct scope *scope, struct expr *arguments) {
  if (arguments == NULL || arguments->next == NULL) {
    fprintf(stderr, "set must have at least three arguments.\n");
    exit(1);
  }

  assert(arguments->type = SYMBOL_EXPR);
  struct expr *val = eval(scope, arguments->next);
  scope_set_value(scope, arguments->data.string_value, val);
  return val;
}
