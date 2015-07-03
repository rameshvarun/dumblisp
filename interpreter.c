#include "lexer.h"
#include "parser.h"
#include "emitter.h"
#include "scope.h"
#include "builtins.h"

#include "stdlib.h"
#include <readline/readline.h>
#include <readline/history.h>

#define REPL_LINE_MAX 4096

// Creates the root scope of the program, populating it with the necessary built-ins.
static struct scope *create_root_scope() {
  struct scope *root = scope_create(NULL);

  scope_add_mapping(root, "print", create_builtin(builtin_print));

  scope_add_mapping(root, "+", create_builtin(builtin_plus));
  scope_add_mapping(root, "-", create_builtin(builtin_sub));
  scope_add_mapping(root, "*", create_builtin(builtin_mult));
  scope_add_mapping(root, "/", create_builtin(builtin_div));

  return root;
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
    printf("\n => ");
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

    struct scope *root = create_root_scope();

    while (true) {
      token_t token;
      get_next_token(&ctx, &token);

      if (token.type == EOF_TOKEN)
        break;

      unget_token(&ctx, &token);
      struct expr *e = parse_expression(&ctx);
      eval(root, e);
    }
  }
}
