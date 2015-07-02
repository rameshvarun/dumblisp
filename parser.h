#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "stdlib.h"
#include "builtins.h"

typedef enum { LIST_EXPR, INT_EXPR, STRING_EXPR, SYMBOL_EXPR, BUILTIN_EXPR } EXPR_TYPE;

// Represents an S-expression
struct expr {
  // The next expression in the current list
  struct expr *next;

  // Whether or not the expression has been 'quoted'
  bool quoted;

  // The type of this expression
  EXPR_TYPE type;

  union {
    // For expressions of type LIST_EXPR, this points to the first element of the list, or NULL if
    // it is
    // an empty list
    struct expr *head;

    // For expressions of type INT_EXPR, this represents it's integer value
    int int_value;

    // For expressions of type STRING_EXPR, this represents the value of the string
    const char *string_value;

    // For expressions of type BUILTIN_EXPR, this represents the corresponding function pointer
    builtin builtin_func;
  } data;
};

struct expr *parse_expression(lexing_context *ctx);

#endif
