#ifndef BUILTINS_H
#define BUILTINS_H

#include "scope.h"
#include "expression.h"

struct expr *builtin_print(struct scope *scope, struct expr *arguments);

// Comparison operators
struct expr *builtin_eq(struct scope *scope, struct expr *arguments);
struct expr *builtin_lt(struct scope *scope, struct expr *arguments);
struct expr *builtin_gt(struct scope *scope, struct expr *arguments);

// Arithmatic operators
struct expr *builtin_plus(struct scope *scope, struct expr *arguments);
struct expr *builtin_mult(struct scope *scope, struct expr *arguments);
struct expr *builtin_sub(struct scope *scope, struct expr *arguments);
struct expr *builtin_div(struct scope *scope, struct expr *arguments);

struct expr *builtin_let(struct scope *scope, struct expr *arguments);
struct expr *builtin_plet(struct scope *scope, struct expr *arguments);

struct expr *builtin_lambda(struct scope *scope, struct expr *arguments);
struct expr *builtin_defun(struct scope *scope, struct expr *arguments);

struct expr *builtin_set(struct scope *scope, struct expr *arguments);

#endif
