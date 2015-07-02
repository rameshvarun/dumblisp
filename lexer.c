#include "lexer.h"

#include <stdlib.h>
#include <strings.h>

void create_file_lexer(lexing_context *ctx, const char *filename) {
  ctx->filename = filename;
  ctx->lineno = 1;
  ctx->errors = 0;
  ctx->source_type = FILE_SOURCE;
  ctx->source.file = fopen(filename, "r");
  ctx->stack = NULL;
}

void create_string_lexer(lexing_context *ctx, const char *source) {
  ctx->filename = "<string>";
  ctx->lineno = 1;
  ctx->errors = 0;
  ctx->source_type = STRING_SOURCE;
  ctx->source.string = source;
  ctx->stack = NULL;
}

static int get_char(lexing_context *ctx) {
  if (ctx->source_type == FILE_SOURCE)
    return getc(ctx->source.file);
  else {
    char c = *(ctx->source.string);
    ctx->source.string++;
    if (c == '\0')
      return EOF;
    else
      return c;
  }
}

static void unget_char(lexing_context *ctx, char c) {
  if (ctx->source_type == FILE_SOURCE)
    ungetc(c, ctx->source.file);
  else
    ctx->source.string--;
}

// Advance the file pointer until after the next newline
static void advance_until_newline(lexing_context *ctx) {
  while (true) {
    const int c = get_char(ctx);
    if (c == EOF || c == '\n')
      return;
  }
}

// Read a string literal, ending on a close-quote
const char *read_string(lexing_context *ctx) {
  char literal[MAX_LITERAL_SIZE];
  int position = 0;
  while (true) {
    const int c = get_char(ctx);
    if (c == '\"') {
      literal[position] = '\0';
      break;
    }

    literal[position] = c;
    ++position;
  }
  return strdup(literal);
}

bool is_validsymbolchar(char c) {
  return is_letter(c) || c == '-' || c == ':' || c == '.' || c == '+' || c == '-' || c == '*' ||
         c == '/';
}

// Read a symbol name, ending on a close-quote
static const char *read_symbol(lexing_context *ctx) {
  char name[MAX_LITERAL_SIZE];
  int position = 0;
  while (true) {
    const int c = get_char(ctx);
    if (is_validsymbolchar(c)) {
      name[position] = c;
      ++position;
    } else {
      name[position] = '\0';
      unget_char(ctx, c);
      break;
    }
  }
  return strdup(name);
}

static int read_integer(lexing_context *ctx) {
  char string[MAX_INTEGER_LITERAL_SIZE];
  int position = 0;
  while (true) {
    const int c = get_char(ctx);
    if (is_digit(c)) {
      string[position] = c;
      ++position;
    } else {
      string[position] = '\0';
      unget_char(ctx, c);
      break;
    }
  }
  return atoi(string);
}

bool is_whitespace(char c) { return (c == ' ' || c == '\n' || c == '\r' || c == '\t'); }

bool is_digit(char c) { return c >= '0' && c <= '9'; }

bool is_letter(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }

bool is_alphanumeric(char c) { return is_letter(c) || is_digit(c); }

void unget_token(lexing_context *ctx, token_t *token) {
  token_t* new_token = malloc(sizeof(token_t));
  memcpy(new_token, token, sizeof(token_t));
  new_token->next = ctx->stack;
  ctx->stack = new_token;
}

// Get the next token starting from the current file cursor
void get_next_token(lexing_context *ctx, token_t *token) {
  if (ctx->stack) {
    token_t *next_tok = ctx->stack;
    ctx->stack = next_tok->next;
    memcpy(token, next_tok, sizeof(token_t));
    free(next_tok);
    return;
  }

  while (true) {
    const int c = get_char(ctx);

    // End of File token
    if (c == EOF) {
      token->type = EOF_TOKEN;
      break;
    }

    // Beginning of a symbol
    if (is_validsymbolchar(c)) {
      unget_char(ctx, c);
      token->type = SYMBOL_TOKEN;
      token->data.string_value = read_symbol(ctx);
      break;
    }

    //  Beginning of an integer
    if (is_digit(c)) {
      unget_char(ctx, c);
      token->type = INT_TOKEN;
      token->data.int_value = read_integer(ctx);
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
      token->type = LEFT_PAREN_TOKEN;
      break;
    }
    if (c == ')') {
      token->type = RIGHT_PAREN_TOKEN;
      break;
    }

    if (c == '\'') {
      token->type = QUOTE_TOKEN;
      break;
    }

    if (c == '\"') {
      token->type = STRING_TOKEN;
      token->data.string_value = read_string(ctx);
      break;
    }

    fprintf(stderr, "Lexer Error in %s:%d - Unkown character '%c'\n", ctx->filename, ctx->lineno,
            c);
  }
  token->lineno = ctx->lineno;
}

const char *get_token_type_name(TOKEN_TYPE type) {
  switch (type) {
  case EOF_TOKEN:
    return "EOF_TOKEN";
  case LEFT_PAREN_TOKEN:
    return "LEFT_PAREN_TOKEN";
  case RIGHT_PAREN_TOKEN:
    return "RIGHT_PAREN_TOKEN";
  case STRING_TOKEN:
    return "STRING_TOKEN";
  case INT_TOKEN:
    return "INT_TOKEN";
  case SYMBOL_TOKEN:
    return "SYMBOL_TOKEN";
  default:
    return "UNKOWN_TYPE";
  }
}

void test_lexer(lexing_context *ctx) {
  // For now simply prints out tokens
  while (true) {
    token_t tok;
    get_next_token(ctx, &tok);

    switch (tok.type) {
    case STRING_TOKEN:
    case SYMBOL_TOKEN:
      printf("%s at %s:%d - %s\n", get_token_type_name(tok.type), ctx->filename, tok.lineno,
             tok.data.string_value);
      break;
    case INT_TOKEN:
      printf("%s at %s:%d - %d\n", get_token_type_name(tok.type), ctx->filename, tok.lineno,
             tok.data.int_value);
      break;
    default:
      printf("%s at %s:%d\n", get_token_type_name(tok.type), ctx->filename, tok.lineno);
    }

    if (tok.type == EOF_TOKEN)
      break;
  }
}
