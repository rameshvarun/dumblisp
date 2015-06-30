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

  // String expression
  if (token.type == STRING_TOKEN) {
    e->type = STRING_EXPR;
    e->data.string_value = token.data.string_value;
  }

  // List expression
  if (token.type == LEFT_PAREN_TOKEN) {
    e->type = LIST_EXPR;
  }

  return e;
}
