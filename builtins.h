#ifndef BUILTINS_H
#define BUILTINS_H

#include "scope.h"
#include "expression.h"

struct expr *builtin_print(struct scope *scope, struct expr *arguments);

#endif
