#pragma once

#include "builtins.h"
#include "defs.h"
#include "expression.h"
#include "lexer.h"
#include "stdlib.h"

struct expr *parse_expression(lexing_context *ctx);