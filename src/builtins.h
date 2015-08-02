#pragma once

#include "scope.h"
#include "expression.h"
#include "defs.h"

/* ESSENTIAL BUILTINS */
// Return the argument directly, without evaluating.
expr *builtin_quote(scope *scope, expr *arguments);
// Exit with a status code. If no code is provided, exit with status code 0.
expr *builtin_exit(scope *scope, expr *arguments);

/* IO functions */
expr *builtin_print(scope *scope, expr *arguments);

/* Control flow */
expr *builtin_if(scope *scope, expr *arguments);
expr *builtin_while(scope *scope, expr *arguments);

/* Integer comparison operators */
expr *builtin_eq(scope *scope, expr *arguments);
expr *builtin_lt(scope *scope, expr *arguments);
expr *builtin_gt(scope *scope, expr *arguments);

// Boolean operators
expr *builtin_or(scope *scope, expr *arguments);
expr *builtin_and(scope *scope, expr *arguments);
expr *builtin_not(scope *scope, expr *arguments);

/* Arithmatic operators */
expr *builtin_plus(scope *scope, expr *arguments);
expr *builtin_mult(scope *scope, expr *arguments);
expr *builtin_sub(scope *scope, expr *arguments);
expr *builtin_div(scope *scope, expr *arguments);

/* Set variables / create new scopes */
expr *builtin_let(scope *scope, expr *arguments);
expr *builtin_plet(scope *scope, expr *arguments);
expr *builtin_set(scope *scope, expr *arguments);

/* Function creation */
expr *builtin_lambda(scope *scope, expr *arguments);
expr *builtin_defun(scope *scope, expr *arguments);
expr *builtin_defmacro(scope *scope, expr *arguments);

// List manipulation
expr *builtin_len(scope *scope, expr *arguments);
expr *builtin_head(scope *scope, expr *arguments);
expr *builtin_tail(scope *scope, expr *arguments);
expr *builtin_nth(scope *scope, expr *arguments);
expr *builtin_list(scope *scope, expr *arguments);
expr *builtin_cons(scope *scope, expr *arguments);
