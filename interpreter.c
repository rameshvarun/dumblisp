#include "lexer.h"
#include "parser.h"
#include "emitter.h"
#include "scope.h"

#include "stdlib.h"
#include <readline/readline.h>
#include <readline/history.h>

#define REPL_LINE_MAX 4096

// Creates the root scope of the program, populating it with the necessary built-ins.
static struct scope *create_root_scope() {
  struct scope *root = scope_create(NULL);
  return root;
}

struct expr *eval(struct scope *scope, struct expr *e) {
  // If this expression is quoted, then evaling it returns
  // the expression, unquoted.
  if (e->quoted) {
    e->quoted = false;
    return e;
  }

  switch (e->type) {
  // String and Integer expressions evaluate to themselves.
  case STRING_EXPR:
  case INT_EXPR:
    return e;
  case SYMBOL_EXPR: {
    struct expr *value = scope_lookup(scope, e->data.string_value);
    if (!value) {
      fprintf(stderr, "Symbol %s not bound to any value\n", e->data.string_value);
      exit(1);
    }
    return NULL;
  }
  default:
    fprintf(stderr, "Unkown expression type.\n");
    exit(1);
    return NULL;
  }
}

void run_repl() {
  struct scope *root = create_root_scope();

  while (true) {
    // Read line from standard in
    char *line = readline(">> ");

    // Create a lexing context on the given line
    lexing_context ctx;
    create_string_lexer(&ctx, line);
    struct expr *e = parse_expression(&ctx);

    // Evaluate expression
    struct expr *result = eval(root, e);

    // Print back out result
    emit_expression(result);
    printf("\n");

    // Save line to history, and free line
    add_history(line);
    free(line);
  }
}

int main(int argc, char **argv) {
  if (argc == 1) // No arguments means run REPL
    run_repl();

  // One argument means a file to load
  if (argc == 2) {
    lexing_context ctx;
    create_file_lexer(&ctx, argv[1]);
    test_lexer(&ctx);
  }
}
