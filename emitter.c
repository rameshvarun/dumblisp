#include "emitter.h"

void emit_expression(struct expr *e) {
  if (e->quoted)
    printf("'");

  switch (e->type) {
  case STRING_EXPR:
    printf("\"%s\"", e->data.string_value);
    break;
  case INT_EXPR:
    printf("%d", e->data.int_value);
    break;
  case SYMBOL_EXPR:
    printf("%s", e->data.string_value);
    break;
  default:
    printf("UNKOWN_EXPR");
    break;
  }
}
