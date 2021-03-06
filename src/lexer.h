#pragma once

#include "defs.h"
#include <stdbool.h>
#include <stdio.h>

// An enum representing the possible tokens in a dumblisp program.
typedef enum {
  LEFT_PAREN_TOKEN,
  RIGHT_PAREN_TOKEN,
  STRING_TOKEN,
  SYMBOL_TOKEN,
  QUOTE_TOKEN,
  EOF_TOKEN,
  INT_TOKEN
} TOKEN_TYPE;

// A token object
typedef struct token {
  struct token *next;

  TOKEN_TYPE type;
  int lineno;
  union {
    const char *string_value;
    int int_value;
  } data;
} token_t;

// A lexing context
typedef struct {
  // The lexer can either read from a file, or from a string
  enum { FILE_SOURCE, STRING_SOURCE } source_type;

  union {
    FILE *file;
    const char *string;
  } source;

  int lineno;
  const char *filename;
  int errors;

  token_t *stack;
} lexing_context;

// Creates a lexing context that reads tokens from the given filename
void create_file_lexer(lexing_context *ctx, const char *filename);

// Creates a lexing context that reads tokens from a given string
void create_string_lexer(lexing_context *ctx, const char *string);

// Gets the next token in the given lexing context
void get_next_token(lexing_context *ctx, token_t *token);

// Pushes a token back into the token stream
void unget_token(lexing_context *ctx, token_t *token);

// Gets the type of the token as a string
const char *get_token_type_name(TOKEN_TYPE type);

// Returns true if c is a whitespace character
bool is_whitespace(char c);

// Returns true if c is a digit
bool is_digit(char c);

// Returns true if c is a lowercase or uppercase digit
bool is_letter(char c);

// Returns true if c is a letter or a number
bool is_alphanumeric(char c);

void test_lexer(lexing_context *ctx);