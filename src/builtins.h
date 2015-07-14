#ifndef BUILTINS_H
#define BUILTINS_H

#include "scope.h"
#include "expression.h"

// IO functions
struct expr *builtin_print(struct scope *scope, struct expr *arguments);

// Control flow
struct expr *builtin_if(struct scope *scope, struct expr *arguments);
struct expr *builtin_while(struct scope *scope, struct expr *arguments);

// Integer comparison operators
struct expr *builtin_eq(struct scope *scope, struct expr *arguments);
struct expr *builtin_lt(struct scope *scope, struct expr *arguments);
struct expr *builtin_gt(struct scope *scope, struct expr *arguments);

// Boolean operators
struct expr *builtin_or(struct scope *scope, struct expr *arguments);

// Arithmatic operators
struct expr *builtin_plus(struct scope *scope, struct expr *arguments);
struct expr *builtin_mult(struct scope *scope, struct expr *arguments);
struct expr *builtin_sub(struct scope *scope, struct expr *arguments);
struct expr *builtin_div(struct scope *scope, struct expr *arguments);

// Set variables / create new scopes
struct expr *builtin_let(struct scope *scope, struct expr *arguments);
struct expr *builtin_plet(struct scope *scope, struct expr *arguments);
struct expr *builtin_set(struct scope *scope, struct expr *arguments);

// Function creation
struct expr *builtin_lambda(struct scope *scope, struct expr *arguments);
struct expr *builtin_defun(struct scope *scope, struct expr *arguments);

#endif
