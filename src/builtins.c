#include "builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* HELPER FUNCTIONS */
static inline bool islist(expr* e) {
  return e == NULL || e->type == CELL_EXPR;
}

static int len(expr *list) {
  assert(islist(list));
  if (list == NULL) return 0;
  else return 1 + len(list->tail);
}

static expr *nth(int n, expr *list) {
  assert(islist(list));
  if (list == NULL)
    return NULL;
  if (n == 0)
    return list->head;
  return nth(n - 1, list->tail);
}

/* ESSENTIAL BUILTINS */
expr *builtin_quote(scope *scope, expr *arguments) {
  if (len(arguments) != 1)
    PANIC("QUOTE must have exactly one argument.");
  return arguments->head;
}

expr *builtin_exit(scope *scope, expr *arguments) {
  if (len(arguments) >= 1) {
    expr *status_code = eval(scope, arguments->head);
    assert(status_code->type == INT_EXPR);
    exit(status_code->int_value);
  } else {
    exit(0);
  }
}

/* IO Utilities */
expr *builtin_print(struct scope *scope, expr *arguments) {
  if (len(arguments) == 0)
    PANIC("PRINT must have at least one argument.\n");

  for (expr *arg = arguments; arg != NULL; arg = arg->tail) {
    expr *value = eval(scope, arg->head);
    switch (value->type) {
    case STRING_EXPR:
      printf("%s", value->string_value);
      break;
    case INT_EXPR:
      printf("%d", value->int_value);
      break;
    case BOOL_EXPR:
      if (value->boolean_value)
        printf("TRUE");
      else
        printf("FALSE");
    default:
      break;
    }

    if (arg->tail != NULL) {
      printf(" ");
    }
  }

  printf("\n");
  return NULL;
}

/* CONTROL FLOW */
expr *builtin_if(struct scope *scope, expr *arguments) {
  int arglen = len(arguments);
  if (arglen < 2)
    PANIC("IF must have at least two arguments.\n");

  expr *cond = eval(scope, nth(0, arguments));
  assert(cond->type = BOOL_EXPR);

  // If the condiiton evaluates to true
  if (cond->boolean_value)
    return eval(scope, nth(1, arguments));

  // If the conditon evaluates to false
  if (arglen > 2)
    return eval(scope, nth(2, arguments));

  return NULL;
}

/*expr *builtin_while(struct scope *scope, expr *arguments) {
  if (arguments == NULL || arguments->tail == NULL)
    PANIC("WHILE must have at least two arguments.\n");

  while (true) {
    // Evaulate the condition.
    expr *cond = eval(scope, arguments);
    assert(cond->type = BOOL_EXPR);

    // If the condiiton evaluates to false, exit loop.
    if (!cond->data.boolean_value)
      break;

    // Otherwise evaluate body statements.
    for (expr *e = arguments->next; e != NULL; e = e->next) {
      eval(scope, e);
    }
  }

  return create_empty_list();
}*/

expr *builtin_or(scope *scope, expr *arguments) {
  if (len(arguments) < 2)
    fprintf(stderr, "OR must have at least two arguments.");

  // TODO: Multi-arg OR
  expr *first = eval(scope, nth(0, arguments));
  expr *second = eval(scope, nth(1, arguments));
  assert(first->type == BOOL_EXPR);
  assert(second->type == BOOL_EXPR);
  return create_bool(first->boolean_value || second->boolean_value);
}

/* COMPARISONS */

expr *builtin_eq(scope *scope, expr *arguments) {
  if (len(arguments) != 2)
    PANIC("= must have at least two arguments.");

  expr *first = eval(scope, arguments->head);
  expr *second = eval(scope, nth(1, arguments));
  assert(first->type == INT_EXPR);
  assert(second->type == INT_EXPR);
  return create_bool(first->int_value == second->int_value);
}

expr *builtin_lt(scope *scope, expr *arguments) {
  if (len(arguments) != 2)
    PANIC("< must have at least two arguments.");

  expr *first = eval(scope, arguments->head);
  expr *second = eval(scope, nth(1, arguments));
  assert(first->type == INT_EXPR);
  assert(second->type == INT_EXPR);
  return create_bool(first->int_value < second->int_value);
}

expr *builtin_gt(scope *scope, expr *arguments) {
  if (len(arguments) != 2)
    PANIC("> must have at least two arguments.");

  expr *first = eval(scope, arguments->head);
  expr *second = eval(scope, nth(1, arguments));
  assert(first->type == INT_EXPR);
  assert(second->type == INT_EXPR);
  return create_bool(first->int_value > second->int_value);
}

/* ARITHMATIC OPERATORS */

expr *builtin_plus(struct scope *scope, expr *arguments) {
  if (len(arguments) == 0)
    PANIC("+ must have at least one argument.\n");

  int current_value = 0;
  for (expr *e = arguments; e != NULL; e = e->tail) {
    expr *value = eval(scope, e->head);
    assert(value->type == INT_EXPR);
    current_value += value->int_value;
  }

  return create_int(current_value);
}

expr *builtin_sub(struct scope *scope, expr *arguments) {
  if (len(arguments) < 2)
    PANIC("- must have at least two arguments.\n");

  expr *first = eval(scope, arguments->head);
  assert(first->type == INT_EXPR);
  int current_value = first->int_value;
  for (expr *e = arguments->tail; e != NULL; e = e->tail) {
    expr *value = eval(scope, e->head);
    assert(value->type == INT_EXPR);
    current_value -= value->int_value;
  }

  return create_int(current_value);
}

expr *builtin_mult(struct scope *scope, expr *arguments) {
  if (len(arguments) == 0)
    PANIC("* must have at least one argument.\n");

  int current_value = 1;
  for (expr *e = arguments; e != NULL; e = e->tail) {
    expr *value = eval(scope, e->head);
    assert(value->type == INT_EXPR);
    current_value *= value->int_value;
  }

  return create_int(current_value);
}

expr *builtin_div(struct scope *scope, expr *arguments) {
  if (len(arguments) < 2)
    PANIC("/ must have at least two arguments.\n");

  expr *first = eval(scope, arguments->head);
  assert(first->type == INT_EXPR);
  int current_value = first->int_value;
  for (expr *e = arguments->tail; e != NULL; e = e->tail) {
    expr *value = eval(scope, e->head);
    assert(value->type == INT_EXPR);
    current_value /= value->int_value;
  }

  return create_int(current_value);
}

/* Set variables / create new scopes */

expr *builtin_let(struct scope *scope, expr *arguments) {
  if (len(arguments) < 2)
    PANIC("LET must have at least two arguments.");

  struct scope *new_scope = scope_create(scope);
  assert(islist(arguments->head)); // The fist argument must be a list.

  for (expr *mapping = arguments->head; mapping != NULL; mapping = mapping->tail) {
    // Each mapping must be a list with two arguments.
    assert(islist(mapping->head));
    assert(len(mapping->head) == 2);

    expr *symb = nth(0, mapping->head);
    assert(symb->type == SYMBOL_EXPR);
    expr *val = eval(new_scope, nth(1, mapping->head));
    scope_add_mapping(new_scope, symb->string_value, val);
  }

  expr *last_value = NULL;
  for (expr *e = arguments->tail; e != NULL; e = e->tail)
    last_value = eval(new_scope, e->head);
  return last_value;
}

expr *builtin_plet(struct scope *scope, expr *arguments) {
  if (len(arguments) < 2)
    PANIC("PLET must have at least two arguments.");

  struct scope *new_scope = scope_create(scope);
  assert(islist(arguments->head)); // The fist argument must be a list.

  for (expr *mapping = arguments->head; mapping != NULL; mapping = mapping->tail) {
    // Each mapping must be a list with two arguments.
    assert(islist(mapping->head));
    assert(len(mapping->head) == 2);

    expr *symb = nth(0, mapping->head);
    assert(symb->type == SYMBOL_EXPR);
    expr *val = eval(scope, nth(1, mapping->head));
    scope_add_mapping(new_scope, symb->string_value, val);
  }

  expr *last_value = NULL;
  for (expr *e = arguments->tail; e != NULL; e = e->tail)
    last_value = eval(new_scope, e->head);
  return last_value;
}

expr *builtin_set(struct scope *scope, expr *arguments) {
  if (len(arguments) != 2)
    PANIC("SET must have two arguments.");

  assert(arguments->head->type = SYMBOL_EXPR);
  expr *val = eval(scope, arguments->tail->head);

  // If a mapping exists in the scope chain, modify that mapping. Otherwise, create new mapping in
  // the current scope.
  if (scope_lookup(scope, arguments->head->string_value) != NULL)
    scope_set_value(scope, arguments->head->string_value, val);
  else
    scope_add_mapping(scope, arguments->head->string_value, val);

  return val;
}

/*

expr *builtin_strlen(struct scope *scope, expr *arguments) { return NULL; }

expr *builtin_strcmp(struct scope *scope, expr *arguments) { return NULL; }

expr *builtin_strcat(struct scope *scope, expr *arguments) { return NULL; }
*/

/* Function Creation */
expr *builtin_lambda(struct scope *scope, expr *arguments) {
  if (len(arguments) < 2)
    PANIC("LAMBDA must have at least two arguments.\n");
  assert(islist(arguments->head));
  return create_func(arguments->head, scope, arguments->tail, false);
}

expr *builtin_defun(struct scope *scope, expr *arguments) {
  if (len(arguments) < 3)
    PANIC("DEFUN must have at least three arguments.\n");

  assert(arguments->head->type == SYMBOL_EXPR);
  assert(islist(nth(2, arguments)));

  if (scope_probe(scope, arguments->head->string_value) == NULL)
    scope_add_mapping(scope, arguments->head->string_value,
                      create_func(nth(1, arguments), scope, arguments->tail->tail, false));
  else
    PANIC("DEFUN tried to create mapping for %s, but one already existed.",
          arguments->head->string_value);
  return NULL;
}

/*
expr *builtin_defmacro(struct scope *scope, expr *arguments) {
  if (len(arguments) < 3)
    PANIC("DEFUN must have at least three arguments.\n");

  assert(arguments->head->type == SYMBOL_EXPR);
  assert(nth(2, arguments)->type == CELL_EXPR);

  scope_add_mapping(
      scope, arguments->data.string_value,
      create_func_expression(arguments->next->data.head, scope, arguments->next->next, true));
  return create_empty_list();
}

*/

// List manipulation
expr *builtin_len(scope *scope, expr *arguments) {
  if (len(arguments) != 1)
    PANIC("LEN must have exactly one argument.");
  return create_int(len(eval(scope, arguments->head)));
}
expr *builtin_head(scope *scope, expr *arguments) {
  if (len(arguments) != 1)
    PANIC("HEAD must have exactly one argument.");
  return eval(scope, arguments->head)->head;
}
expr *builtin_tail(scope *scope, expr *arguments) {
  if (len(arguments) != 1)
    PANIC("TAIL must have exactly one argument.");
  return eval(scope, arguments->head)->tail;
}
expr *builtin_nth(scope *scope, expr *arguments) {
  if (len(arguments) != 2)
    PANIC("NTH must have exactly two arguments.");
  expr *n_expr = eval(scope, nth(0, arguments));
  expr *list_expr = eval(scope, nth(1, arguments));
  
  assert(n_expr->type == INT_EXPR); // First arg must evaluate to an integer.
  assert(islist(list_expr)); // Second arg must evaluate to list.

  return nth(n_expr->int_value, list_expr);
}
expr *builtin_list(scope *scope, expr *arguments) {
  if (arguments == NULL)
    return NULL;
  return create_cell(eval(scope, arguments->head), builtin_list(scope, arguments->tail));
}
expr *builtin_cons(scope *scope, expr *arguments) {
  if (len(arguments) != 2)
    PANIC("CONS must have exactly one argument.");
  return create_cell(eval(scope, nth(0, arguments)), eval(scope, nth(1, arguments)));
}