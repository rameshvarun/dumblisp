#ifndef BUILTINS_H
#define BUILTINS_H

#include "scope.h"
#include "expression.h"

struct expr *builtin_puts(struct scope *scope, struct expr *arguments);

#endif
