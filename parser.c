#include "parser.h"

#include <strings.h>

// Recursive-descent parser
struct expr *parse_expression(lexing_context *ctx) {
  token_t token;
  get_next_token(ctx, &token);

  if (token.type == QUOTE_TOKEN) {
    struct expr *e = parse_expression(ctx);
    e->quoted = true;
    return e;
  }

  // Malloc a new expression object
  struct expr *e = malloc(sizeof(struct expr));
  memset(e, 0, sizeof(struct expr));

  // String atom
  if (token.type == STRING_TOKEN) {
    e->type = STRING_EXPR;
    e->data.string_value = token.data.string_value;
  }

  // Integer atom
  if (token.type == INT_TOKEN) {
    e->type = INT_EXPR;
    e->data.int_value = token.data.int_value;
  }

  // Symbol
  if (token.type == SYMBOL_TOKEN) {
    e->type = SYMBOL_EXPR;
    e->data.string_value = token.data.string_value;
  }

  // List expression
  /*if (token.type == LEFT_PAREN_TOKEN) {
    e->type = LIST_EXPR;
    abort();
  }*/

  // End of a list
  /*if (token.type == RIGHT_PAREN_TOKEN) {
    return NULL;
   }*/

  return e;
}
