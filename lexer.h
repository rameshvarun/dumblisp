#ifndef LEXER_H
#define LEXER_H

#define MAX_LITERAL_SIZE 4096

#include <stdio.h>
#include <stdbool.h>
#include <strings.h>

typedef enum {
  LEFT_PAREN,
  RIGHT_PAREN,
  STRING,
  SYMBOL,
  QUOTE,
  EOF_TOKEN
} TOKEN_TYPE;

typedef struct {
  TOKEN_TYPE type;
  int lineno;

  union {
    const char *string_value;
    int int_value;
  } data;
} token_t;

typedef struct {
  FILE *file;
  int lineno;
  const char *filename;
  int errors;
} lexing_context;

void create_lexing_context(lexing_context *ctx, const char *filename);
void get_next_token(lexing_context *ctx, token_t *token);
const char *get_token_type_name(TOKEN_TYPE type);
bool is_whitespace(char c);
bool is_digit(char c);
bool is_letter(char c);
bool is_alphanumeric(char c);

#endif
