#pragma once

#include "lexer.h"
#include "stdlib.h"
#include "builtins.h"
#include "expression.h"
#include "defs.h"

struct expr *parse_expression(lexing_context *ctx);