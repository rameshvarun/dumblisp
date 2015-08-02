#include "emitter.h"

void emit_expression(expr *e) {
  // A NULL pointer is the same as the empty list.
  if (e == NULL) {
    printf("()");
    return;
  }

  switch (e->type) {
  // String expressions are printed out in quotes
  case STRING_EXPR:
    printf("\"%s\"", e->string_value);
    break;

  // Int expressions are directly printed
  case INT_EXPR:
    printf("%d", e->int_value);
    break;

  // Boolean expressions either print TRUE or FALSE
  case BOOL_EXPR:
    if (e->boolean_value)
      printf("TRUE");
    else
      printf("FALSE");
    break;

  // Symbols are directly printed
  case SYMBOL_EXPR:
    printf("%s", e->string_value);
    break;

  // Lists are printed, wrapped in parentheses
  case CELL_EXPR:
    printf("(");
    for (struct expr *curr = e; curr != NULL; curr = curr->tail) {
      emit_expression(curr->head);
      if (curr->tail != NULL)
        printf(" ");
    }
    printf(")");
    break;

  // Builtin-in function
  case BUILTIN_EXPR:
    printf("<built-in function>");
    break;

  // User-defined function object
  case FUNC_EXPR:
    printf("<function args: ");
    emit_expression(e->arguments);
    printf(" body: ");
    emit_expression(e->body);
    printf(">");
    break;
  // Unkown expression type
  default:
    printf("UNKOWN_EXPR");
    break;
  }
}
