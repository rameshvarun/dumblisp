#ifndef BUILTINS_H
#define BUILTINS_H

#include "scope.h"
#include "parser.h"

typedef void (*builtin)(struct scope *scope, struct expr *arguments);

#endif
