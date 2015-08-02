#include "parser.h"
#include "stringpool.h"
#include <strings.h>

// Recursive helper funtion for parsing a list.
static expr *parse_list(lexing_context *ctx) {
  // Get a single token.
  token_t token;
  get_next_token(ctx, &token);

  // If the token is a ')', simply return NULL, as this is the end of the list.
  if (token.type == RIGHT_PAREN_TOKEN)
    return NULL;
  // If not, push the token pack onto the lexer stack.
  unget_token(ctx, &token);

  expr *head = parse_expression(ctx);
  expr *tail = parse_list(ctx);
  return create_cell(head, tail);
}

// Implementation of a recursive-descent parser
struct expr *parse_expression(lexing_context *ctx) {
  // Start by getting the next token from the lexing context
  token_t token;
  get_next_token(ctx, &token);

  switch (token.type) {
  case EOF_TOKEN:
    PANIC("Parse Error: Unexpected EOF token\n");
  case QUOTE_TOKEN:
    return create_cell(create_symbol(stringpool_add("QUOTE")),
                       create_cell(parse_expression(ctx), NULL));
  case STRING_TOKEN:
    return create_string(token.data.string_value);
  case INT_TOKEN:
    return create_int(token.data.int_value);
  case SYMBOL_TOKEN:
    return create_symbol(token.data.string_value);
  case LEFT_PAREN_TOKEN:
    return parse_list(ctx);
  default:
    PANIC("Encountered unkown token type while parsing.\n");
  }
}
