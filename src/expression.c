#include "expression.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "assert.h"
#include "emitter.h"

static inline expr *make_expr() {
  expr *e = malloc(sizeof(expr));
  memset(e, 0, sizeof(expr));
  return e;
}

expr *create_cell(expr *head, expr *tail) {
  expr *e = make_expr();
  e->type = CELL_EXPR;
  e->head = head;
  e->tail = tail;
  return e;
}

expr *create_int(int value) {
  expr *e = make_expr();
  e->type = INT_EXPR;
  e->int_value = value;
  return e;
}

expr *create_bool(bool value) {
  expr *e = make_expr();
  e->type = BOOL_EXPR;
  e->boolean_value = value;
  return e;
}

expr *create_string(const char *value) {
  expr *e = make_expr();
  e->type = STRING_EXPR;
  e->string_value = value;
  return e;
}

expr *create_symbol(const char *value) {
  expr *e = make_expr();
  e->type = SYMBOL_EXPR;
  e->string_value = value;
  return e;
}

expr *create_builtin(builtin func_ptr) {
  expr *e = make_expr();
  e->type = BUILTIN_EXPR;
  e->func_ptr = func_ptr;
  return e;
}

expr *create_func(expr *arguments, struct scope *closure, expr *body, bool ismacro) {
  expr *e = make_expr();
  e->type = FUNC_EXPR;
  e->arguments = arguments;
  e->closure = closure;
  e->body = body;
  e->ismacro = ismacro;
  return e;
}

expr *eval(scope *scope, expr *e) {
  if (e == NULL)
    return NULL;

  switch (e->type) {
  // String and Integer expressions evaluate to themselves.
  case STRING_EXPR:
  case INT_EXPR:
    return e;
  case SYMBOL_EXPR: {
    expr *value = scope_lookup(scope, e->string_value);
    if (!value)
      PANIC("Symbol %s not bound to any value\n", e->string_value);
    return value;
  }
  case CELL_EXPR: {
    expr *head = eval(scope, e->head);
    if (head->type == BUILTIN_EXPR) {
      // Call the built-in construct.
      return head->func_ptr(scope, e->tail);
    } else if (head->type == FUNC_EXPR) {
      // Call the function.
      struct scope *new_scope = scope_create(head->closure);

      expr *actuals = e->tail;
      for (expr *formal = head->arguments; formal != NULL; formal = formal->tail) {
        expr *formal_expr = formal->head;
        assert(formal_expr->type == SYMBOL_EXPR);
        if (actuals != NULL) {
          // If this is a regular function, evaluate the argument. Otherwise, return expression,
          // Since macros take in their arguments literally.
          expr *actual_value = head->ismacro ? actuals->head : eval(scope, actuals->head);
          // Bind the actual value to the formal symbol.
          scope_add_mapping(new_scope, formal_expr->string_value, actual_value);
          // Proceed to the next actual.
          actuals = actuals->tail;
        } else {
          // No more actuals, so simply bind the symbol to the empty list (NIL).
          scope_add_mapping(new_scope, formal_expr->string_value, NULL);
        }
      }

      // Evaulate the body of the function
      struct expr *last_value = NULL;
      for (struct expr *statement = head->body; statement != NULL; statement = statement->tail) {
        last_value = eval(new_scope, statement->head);
      }

      // If this is a regular function, we simply return the result.
      return head->ismacro ? eval(scope, last_value) : last_value;
    } else {
      abort();
    }
  }
  default:
    PANIC("Unkown expression type.\n");
  }
}
