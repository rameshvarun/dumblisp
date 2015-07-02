#ifndef EMITTER_H
#define EMITTER_H

#include "parser.h"

// Prints out a representation of the given expression to stdout.
// Used as part of the REPL.
void emit_expression(struct expr *e);

#endif
