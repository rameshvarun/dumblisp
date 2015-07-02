#include "parser.h"

#include <strings.h>

// Implementation of a recursive-descent parser
struct expr *parse_expression(lexing_context *ctx) {
  // Start by getting the next token from the lexing context
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

  // Symbol atom
  if (token.type == SYMBOL_TOKEN) {
    e->type = SYMBOL_EXPR;
    e->data.string_value = token.data.string_value;
  }

  // List expression
  if (token.type == LEFT_PAREN_TOKEN) {
    e->type = LIST_EXPR;

    struct expr *current = NULL;
    while (true) {
      // Try to parse the next element of the list
      struct expr *next = parse_expression(ctx);

      // If there are no more elements, end the loop
      if (next == NULL)
        break;

      if (current == NULL) {
        // If we haven't seen any elements yet, then next is the head of the loop.
        e->data.head = next;
      } else {
        // Otherwise, next is the "next" of the current element
        current->next = next;
      }

      // Advance the current element
      current = next;
    }
  }

  // End of a list
  if (token.type == RIGHT_PAREN_TOKEN) {
    return NULL;
  }

  return e;
}
