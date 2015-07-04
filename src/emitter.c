#include "emitter.h"

void emit_expression(struct expr *e) {
  if (e->quoted)
    printf("'");

  switch (e->type) {
  // String expressions are printed out in quotes
  case STRING_EXPR:
    printf("\"%s\"", e->data.string_value);
    break;

  // Int expressions are directly printed
  case INT_EXPR:
    printf("%d", e->data.int_value);
    break;

  // Symbols are directly printed
  case SYMBOL_EXPR:
    printf("%s", e->data.string_value);
    break;

  // Lists are printed, wrapped in parentheses
  case LIST_EXPR:
    printf("(");
    for (struct expr *curr = e->data.head; curr != NULL; curr = curr->next) {
      emit_expression(curr);
      if (curr->next != NULL)
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
    printf("<function args: (");
    for (struct expr *arg = e->data.function_value.arguments; arg != NULL; arg = arg->next) {
      emit_expression(arg);
      if (arg->next != NULL)
        printf(" ");
    }
    printf(") body: (");
    for (struct expr *curr = e->data.function_value.body; curr != NULL; curr = curr->next) {
      emit_expression(curr);
      if (curr->next != NULL)
        printf(" ");
    }
    printf(")>");
    break;
  // Unkown expression type
  default:
    printf("UNKOWN_EXPR");
    break;
  }
}
