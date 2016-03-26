#pragma once

struct expr;

#include "defs.h"
#include "scope.h"
#include "stdbool.h"

typedef enum {
  CELL_EXPR,
  INT_EXPR,
  STRING_EXPR,
  SYMBOL_EXPR,
  BUILTIN_EXPR,
  FUNC_EXPR,
  BOOL_EXPR,
  USERDATA_EXPR /* Represents a pointer to some user managed data. Similar to lightuserdata. in the
                   Lua programming language. */
} EXPR_TYPE;

// The prototype of a built-in function
typedef struct expr *(*builtin)(struct scope *scope, struct expr *arguments);

// The prototype of a userdata_gc function
typedef void (*userdata_gc)(void *userdata);

// Represents an expression
typedef struct expr {
  // The type of this expression
  EXPR_TYPE type;

  union {
    // For expressions of type CELL_EXPR, this points to the first element of the list, or
    // NULL if it is an empty list
    struct {
      struct expr *head; // The first element in a list
      struct expr *tail; // The rest of a list
    };

    // For expressions of type INT_EXPR, this represents it's integer value
    int int_value;

    // For expressions of type STRING_EXPR or SYMBOL_EXPR, this represents the value of the string
    const char *string_value;

    // For expressions of type BUILTIN_EXPR, this represents the corresponding function pointer
    builtin func_ptr;

    /* For expressions of type FUNC_EXPR, this represents all of the data required to execute the
       function. */
    struct {
      struct expr *arguments; // A list of symbols naming the argument of a function
      struct scope *closure;  // The scope in which this function was created
      struct expr *body;      // A pointer to the first instruction in the body
      bool ismacro;           // Whether or not this function is an fexpr
    };

    // For expressions of type BOOL_EXPR, this represents the boolean value
    bool boolean_value;

    /* For expressions of type USERDATA_EXPR, this stores both the pointer to the user data, and a
     * function that can be used to free / deinitalize that data when this object is garbage
     * collected. */
    struct {
      void *userdata_ptr;
      userdata_gc userdata_gc;
    };
  };
} expr;

// Helper functions for construction expressions.
expr *create_cell(expr *head, expr *tail);
expr *create_builtin(builtin func_ptr);
expr *create_int(int value);
expr *create_bool(bool value);
expr *create_string(const char *value);
expr *create_symbol(const char *value);
expr *create_func(expr *arguments, scope *closure, expr *body, bool ismacro);

bool islist(expr *e);
bool issymbol(expr *e);

// Evaluate an expression in the given scope.
expr *eval(scope *scope, expr *e);
