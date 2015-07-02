#ifndef EXPRESSION_H
#define EXPRESSION_H

struct expr;

#include "stdbool.h"
#include "scope.h"

typedef enum { LIST_EXPR, INT_EXPR, STRING_EXPR, SYMBOL_EXPR, BUILTIN_EXPR, FUNC_EXPR } EXPR_TYPE;

typedef struct expr *(*builtin)(struct scope *scope, struct expr *arguments);

// Represents an expression
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

    // For expressions of type STRING_EXPR or SYMBOL_EXPR, this represents the value of the string
    const char *string_value;

    // For expressions of type BUILTIN_EXPR, this represents the corresponding function pointer
    builtin func_ptr;
  } data;
};

struct expr *create_builtin(builtin func_ptr);
struct expr *create_empty_list();
struct expr *create_int_expression(int value);
struct expr *eval(struct scope *scope, struct expr *e);

#endif
