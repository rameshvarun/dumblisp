#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "stdlib.h"
#include "builtins.h"
#include "expression.h"

struct expr *parse_expression(lexing_context *ctx);

#endif
