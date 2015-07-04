#include "expression.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "assert.h"

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

struct expr *create_int_expression(int value) {
  // Malloc a new expression object
  struct expr *e = malloc(sizeof(struct expr));
  memset(e, 0, sizeof(struct expr));
  e->type = INT_EXPR;
  e->data.int_value = value;
  return e;
}

struct expr *create_func_expression(struct expr *arguments, struct scope *closure,
                                    struct expr *body) {
  // Malloc a new expression object
  struct expr *e = malloc(sizeof(struct expr));
  memset(e, 0, sizeof(struct expr));

  e->type = FUNC_EXPR;
  e->data.function_value.arguments = arguments;
  e->data.function_value.closure = closure;
  e->data.function_value.body = body;

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
        // Call the built-in construct.
        return head->data.func_ptr(scope, e->data.head->next);
      } else if (head->type == FUNC_EXPR) {
        // Call the function
        struct scope *new_scope = scope_create(head->data.function_value.closure);

        struct expr *actuals = e->data.head->next;
        for (struct expr *formal = head->data.function_value.arguments; formal != NULL;
             formal = formal->next) {
          assert(formal->type == SYMBOL_EXPR);
          if (actuals != NULL) {
            scope_add_mapping(new_scope, formal->data.string_value, eval(scope, actuals));
            actuals = actuals->next;
          } else {
            scope_add_mapping(new_scope, formal->data.string_value, create_empty_list());
          }
        }

        struct expr *last_value = create_empty_list();
        for (struct expr *statement = head->data.function_value.body; statement != NULL;
             statement = statement->next) {
          last_value = eval(new_scope, statement);
        }
        return last_value;
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
