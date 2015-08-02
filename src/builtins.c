#include "builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* HELPER FUNCTIONS */
static int len(expr *list) {
  if (list == NULL)
    return 0;
  else {
    assert(list->type == CELL_EXPR);
    return 1 + len(list->tail);
  }
}

static expr *nth(int n, expr *list) {
  if (list == NULL)
    return NULL;

  assert(list->type == CELL_EXPR);
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
/*expr *builtin_if(struct scope *scope, expr *arguments) {
  if (arguments == NULL || arguments->tail == NULL)
    PANIC("IF must have at least two arguments.\n");

  expr *cond = eval(scope, arguments->head);
  assert(cond->type = BOOL_EXPR);

  // If the condiiton evaluates to true
  if (cond->data.boolean_value) {
    return eval(scope, arguments->next);
  }

  // If the conditon evaluates to false
  if (arguments->next->next != NULL) {
    return eval(scope, arguments->next->next);
  }

  return NULL;
}*/

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
}

expr *builtin_or(struct scope *scope, expr *arguments) {
  if (arguments == NULL || arguments->next == NULL) {
    fprintf(stderr, "= must have at least two arguments.");
  }
  expr *first = eval(scope, arguments);
  expr *second = eval(scope, arguments->next);
  assert(first->type == BOOL_EXPR);
  assert(second->type == BOOL_EXPR);
  return create_bool_expression(first->data.boolean_value || second->data.boolean_value);
}*/

/* COMPARISONS */

/*expr *builtin_eq(struct scope *scope, expr *arguments) {
  if (arguments == NULL || arguments->next == NULL) {
    fprintf(stderr, "= must have at least two arguments.");
  }
  expr *first = eval(scope, arguments);
  expr *second = eval(scope, arguments->next);
  assert(first->type == INT_EXPR);
  assert(second->type == INT_EXPR);
  return create_bool_expression(first->data.int_value == second->data.int_value);
}

expr *builtin_lt(struct scope *scope, expr *arguments) {
  if (arguments == NULL || arguments->next == NULL) {
    fprintf(stderr, "= must have at least two arguments.");
  }
  expr *first = eval(scope, arguments);
  expr *second = eval(scope, arguments->next);
  assert(first->type == INT_EXPR);
  assert(second->type == INT_EXPR);
  return create_bool_expression(first->data.int_value < second->data.int_value);
}

expr *builtin_gt(struct scope *scope, expr *arguments) {
  if (arguments == NULL || arguments->next == NULL) {
    fprintf(stderr, "= must have at least two arguments.");
  }
  expr *first = eval(scope, arguments);
  expr *second = eval(scope, arguments->next);
  assert(first->type == INT_EXPR);
  assert(second->type == INT_EXPR);
  return create_bool_expression(first->data.int_value > second->data.int_value);
}*/

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

/*

expr *builtin_let(struct scope *scope, expr *arguments) {
  if (arguments == NULL) {
    fprintf(stderr, "let must have at least one argument.\n");
    exit(1);
  }

  struct scope *new_scope = scope_create(scope);
  assert(arguments->type == LIST_EXPR);

  for (expr *mapping = arguments->data.head; mapping != NULL; mapping = mapping->next) {
    assert(mapping->type == LIST_EXPR);

    // Must have two arguments
    assert(mapping->data.head != NULL);
    assert(mapping->data.head->next != NULL);

    expr *symb = mapping->data.head;
    assert(symb->type == SYMBOL_EXPR);
    expr *val = eval(new_scope, mapping->data.head->next);
    scope_add_mapping(new_scope, symb->data.string_value, val);

    // TODO: Ensure that a symbol doesn't repeat
  }

  expr *last_value = create_empty_list();
  for (expr *e = arguments->next; e != NULL; e = e->next) {
    last_value = eval(new_scope, e);
  }

  return last_value;
}

expr *builtin_plet(struct scope *scope, expr *arguments) {
  if (arguments == NULL) {
    fprintf(stderr, "let must have at least one argument.\n");
    exit(1);
  }

  struct scope *new_scope = scope_create(scope);
  assert(arguments->type == LIST_EXPR);

  for (expr *mapping = arguments->data.head; mapping != NULL; mapping = mapping->next) {
    assert(mapping->type == LIST_EXPR);

    // Must have two arguments
    assert(mapping->data.head != NULL);
    assert(mapping->data.head->next != NULL);

    expr *symb = mapping->data.head;
    assert(symb->type == SYMBOL_EXPR);
    expr *val = eval(new_scope, mapping->data.head->next);
    scope_add_mapping(new_scope, symb->data.string_value, val);

    // TODO: Ensure that a symbol doesn't repeat
  }

  expr *last_value = create_empty_list();
  for (expr *e = arguments->next; e != NULL; e = e->next) {
    last_value = eval(scope, e);
  }

  return last_value;
}

expr *builtin_exit(struct scope *scope, expr *arguments) { return NULL; }

expr *builtin_strlen(struct scope *scope, expr *arguments) { return NULL; }

expr *builtin_strcmp(struct scope *scope, expr *arguments) { return NULL; }

expr *builtin_strcat(struct scope *scope, expr *arguments) { return NULL; }

expr *builtin_lambda(struct scope *scope, expr *arguments) {
  if (arguments == NULL || arguments->next == NULL) {
    fprintf(stderr, "lambda must have at least two arguments.\n");
    exit(1);
  }

  assert(arguments->type == LIST_EXPR);
  return create_func_expression(arguments->data.head, scope, arguments->next, false);
}

expr *builtin_defun(struct scope *scope, expr *arguments) {
  if (arguments == NULL || arguments->next == NULL || arguments->next->next == NULL) {
    fprintf(stderr, "defun must have at least three arguments.\n");
    exit(1);
  }

  assert(arguments->type == SYMBOL_EXPR);
  assert(arguments->next->type == LIST_EXPR);

  scope_add_mapping(
      scope, arguments->data.string_value,
      create_func_expression(arguments->next->data.head, scope, arguments->next->next, false));
  return create_empty_list();
}

expr *builtin_defmacro(struct scope *scope, expr *arguments) {
  if (arguments == NULL || arguments->next == NULL || arguments->next->next == NULL) {
    fprintf(stderr, "defun must have at least three arguments.\n");
    exit(1);
  }

  assert(arguments->type == SYMBOL_EXPR);
  assert(arguments->next->type == LIST_EXPR);

  scope_add_mapping(
      scope, arguments->data.string_value,
      create_func_expression(arguments->next->data.head, scope, arguments->next->next, true));
  return create_empty_list();
}

expr *builtin_set(struct scope *scope, expr *arguments) {
  if (arguments == NULL || arguments->next == NULL) {
    fprintf(stderr, "set must have at least three arguments.\n");
    exit(1);
  }

  assert(arguments->type = SYMBOL_EXPR);
  expr *val = eval(scope, arguments->next);
  scope_set_value(scope, arguments->data.string_value, val);
  return val;
}*/

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
    PANIC("NTH must have exactly one argument.");
  expr *n_expr = eval(scope, nth(0, arguments));
  expr *list_expr = eval(scope, nth(1, arguments));
  assert(n_expr->type == INT_EXPR);
  assert(list_expr->type == CELL_EXPR);
  return nth(n_expr->int_value, eval(scope, list_expr));
}
expr *builtin_list(scope *scope, expr *arguments) {
  if (arguments == NULL)
    return NULL;
  return create_cell(eval(scope, arguments->head), builtin_list(scope, arguments->tail));
}
