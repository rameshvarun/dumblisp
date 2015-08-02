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

  scope_add_mapping(root, "QUOTE", create_builtin(builtin_quote));
  scope_add_mapping(root, "EXIT", create_builtin(builtin_exit));

  // Control flow
  // scope_add_mapping(root, "IF", create_builtin(builtin_if));
  // scope_add_mapping(root, "WHILE", create_builtin(builtin_while));

  // Boolean Operators
  // scope_add_mapping(root, "OR", create_builtin(builtin_or));

  // TRUE and FALSE booleans
  scope_add_mapping(root, "TRUE", create_bool(true));
  scope_add_mapping(root, "FALSE", create_bool(false));

  // Create functions and macros
  // scope_add_mapping(root, "DEFUN", create_builtin(builtin_defun));
  // scope_add_mapping(root, "LAMBDA", create_builtin(builtin_lambda));
  // scope_add_mapping(root, "DEFMACRO", create_builtin(builtin_defmacro));

  // Create and manipulate scopes
  // scope_add_mapping(root, "LET", create_builtin(builtin_let));
  // scope_add_mapping(root, "PLET", create_builtin(builtin_plet));
  // scope_add_mapping(root, "SET", create_builtin(builtin_set));

  // I/O Functions
  scope_add_mapping(root, "PRINT", create_builtin(builtin_print));

  // Arithmatic operators
  scope_add_mapping(root, "+", create_builtin(builtin_plus));
  scope_add_mapping(root, "-", create_builtin(builtin_sub));
  scope_add_mapping(root, "*", create_builtin(builtin_mult));
  scope_add_mapping(root, "/", create_builtin(builtin_div));

  // Comparison operators
  scope_add_mapping(root, "=", create_builtin(builtin_eq));
  scope_add_mapping(root, "<", create_builtin(builtin_lt));
  scope_add_mapping(root, ">", create_builtin(builtin_gt));

  // List manipulation
  scope_add_mapping(root, "LEN", create_builtin(builtin_len));
  scope_add_mapping(root, "HEAD", create_builtin(builtin_head));
  scope_add_mapping(root, "TAIL", create_builtin(builtin_tail));
  scope_add_mapping(root, "LIST", create_builtin(builtin_list));

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
