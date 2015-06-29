#include "lexer.h"

void create_lexing_context(lexing_context *ctx, const char *filename) {
  ctx->filename = filename;
  ctx->lineno = 1;
  ctx->errors = 0;
  ctx->file = fopen(filename, "r");
}

// Advance the file pointer until after the next newline
void advance_until_newline(lexing_context *ctx) {
  while (true) {
    const int c = getc(ctx->file);
    if (c == EOF || c == '\n')
      return;
  }
}

// Read a string literal, ending on a close-quote
const char *read_string(lexing_context *ctx) {
  char literal[MAX_LITERAL_SIZE];
  int position = 0;
  while (true) {
    const int c = getc(ctx->file);
    if (c == '\"') {
      literal[position] = '\0';
      break;
    }

    literal[position] = c;
    ++position;
  }
  return strdup(literal);
}

// Read a symbol name, ending on a close-quote
const char *read_symbol(lexing_context *ctx) {
  char name[MAX_LITERAL_SIZE];
  int position = 0;
  while (true) {
    const int c = getc(ctx->file);
    if (is_letter(c) || c == '-' || c == ':') {
      name[position] = c;
      ++position;
    } else {
      name[position] = '\0';
      ungetc(c, ctx->file);
      break;
    }
  }
  return strdup(name);
}

bool is_whitespace(char c) {
  return (c == ' ' || c == '\n' || c == '\r' || c == '\t');
}

bool is_digit(char c) { return c >= '0' && c <= '9'; }

bool is_letter(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool is_alphanumeric(char c) { return is_letter(c) || is_digit(c); }

// Get the next token starting from the current file cursor
void get_next_token(lexing_context *ctx, token_t *token) {
  while (true) {
    const int c = getc(ctx->file);

    // End of File token
    if (c == EOF) {
      token->type = EOF_TOKEN;
      break;
    }

    // Beginning of a symbol
    if (is_alphanumeric(c)) {
      ungetc(c, ctx->file);
      token->type = SYMBOL;
      token->data.string_value = read_symbol(ctx);
      break;
    }

    // Ignore whitespace
    if (is_whitespace(c)) {
      // Augment line number if we pass a newline
      if (c == '\n')
        ++ctx->lineno;
      continue;
    }

    // Single-line comment
    if (c == ';') {
      advance_until_newline(ctx);
      continue;
    }

    // Parentheses
    if (c == '(') {
      token->type = LEFT_PAREN;
      break;
    }
    if (c == ')') {
      token->type = RIGHT_PAREN;
      break;
    }

    if (c == '\'') {
      token->type = QUOTE;
      break;
    }

    if (c == '\"') {
      token->type = STRING;
      token->data.string_value = read_string(ctx);
      break;
    }

    fprintf(stderr, "Lexer Error in %s:%d - Unkown character '%c'\n",
            ctx->filename, ctx->lineno, c);
  }
  token->lineno = ctx->lineno;
}

const char *get_token_type_name(TOKEN_TYPE type) {
  switch (type) {
  case EOF_TOKEN:
    return "EOF_TOKEN";
  case LEFT_PAREN:
    return "LEFT_PAREN";
  case RIGHT_PAREN:
    return "RIGHT_PAREN";
  case STRING:
    return "STRING";
  case SYMBOL:
    return "SYMBOL";
  default:
    return "UNKOWN_TYPE";
  }
}
