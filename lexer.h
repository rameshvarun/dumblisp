#ifndef LEXER_H
#define LEXER_H

// Maximum string and symbol name sizes (for simplicity)
#define MAX_LITERAL_SIZE 4096
#define MAX_INTEGER_LITERAL_SIZE 4096
#define MAX_SYMBOL_SIZE 4096

#include <stdio.h>
#include <stdbool.h>

// An enum representing the possible tokens in a dumblisp program
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
typedef struct {
  TOKEN_TYPE type;
  int lineno;
  union {
    const char *string_value;
    int int_value;
  } data;
} token_t;

// A lexing context
typedef struct {
  FILE *file;
  int lineno;
  const char *filename;
  int errors;
} lexing_context;

// Creates a lexing context that reads tokens from the given filename
void create_lexing_context(lexing_context *ctx, const char *filename);

// Gets the next token in the given lexing context
void get_next_token(lexing_context *ctx, token_t *token);

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

#endif
