#include "lexer.h"
#include "parser.h"
#include "emitter.h"
#include "scope.h"
#include "builtins.h"
#include "lib.h"

#include "stdlib.h"
#include "stringpool.h"

#include <readline/readline.h>
#include <readline/history.h>

#include "gc.h"

#define REPL_LINE_MAX 4096

expr *loadfile(scope *scope, const char *filename) {
  lexing_context ctx;
  create_file_lexer(&ctx, filename);

  expr *last_value = NULL;
  while (true) {
    token_t token;
    get_next_token(&ctx, &token);

    if (token.type == EOF_TOKEN)
      break;

    unget_token(&ctx, &token);
    struct expr *e = parse_expression(&ctx);
    last_value = eval(scope, e);
  }
  return last_value;
}

expr *loadstring(scope *scope, const char *source) {
  lexing_context ctx;
  create_string_lexer(&ctx, source);

  expr *last_value = NULL;
  while (true) {
    token_t token;
    get_next_token(&ctx, &token);

    if (token.type == EOF_TOKEN)
      break;

    unget_token(&ctx, &token);
    struct expr *e = parse_expression(&ctx);
    last_value = eval(scope, e);
  }
  return last_value;
}

// Creates the root scope of the program, populating it with the necessary built-ins.
static struct scope *create_root_scope() {
  struct scope *root = scope_create(NULL);

  scope_add_mapping(root, stringpool_add("QUOTE"), create_builtin(builtin_quote));
  scope_add_mapping(root, stringpool_add("EVAL"), create_builtin(builtin_eval));
  scope_add_mapping(root, stringpool_add("EXIT"), create_builtin(builtin_exit));

  scope_add_mapping(root, stringpool_add("TYPEOF"), create_builtin(builtin_typeof));

  scope_add_mapping(root, stringpool_add("LOAD-STRING"), create_builtin(builtin_loadstring));
  scope_add_mapping(root, stringpool_add("LOAD-FILE"), create_builtin(builtin_loadfile));

  // Control flow
  scope_add_mapping(root, stringpool_add("IF"), create_builtin(builtin_if));
  scope_add_mapping(root, stringpool_add("WHILE"), create_builtin(builtin_while));

  // Boolean Operators
  scope_add_mapping(root, stringpool_add("OR"), create_builtin(builtin_or));

  // TRUE and FALSE booleans
  scope_add_mapping(root, stringpool_add("TRUE"), create_bool(true));
  scope_add_mapping(root, stringpool_add("FALSE"), create_bool(false));

  // Create functions and macros
  scope_add_mapping(root, stringpool_add("DEFUN"), create_builtin(builtin_defun));
  scope_add_mapping(root, stringpool_add("LAMBDA"), create_builtin(builtin_lambda));
  scope_add_mapping(root, stringpool_add("DEFMACRO"), create_builtin(builtin_defmacro));

  // Create and manipulate scopes
  scope_add_mapping(root, stringpool_add("LET"), create_builtin(builtin_let));
  scope_add_mapping(root, stringpool_add("PLET"), create_builtin(builtin_plet));
  scope_add_mapping(root, stringpool_add("SET"), create_builtin(builtin_set));

  // I/O Functions
  scope_add_mapping(root, stringpool_add("PRINT"), create_builtin(builtin_print));
  scope_add_mapping(root, stringpool_add("READ-LINE"), create_builtin(builtin_readline));

  // Arithmatic operators
  scope_add_mapping(root, stringpool_add("+"), create_builtin(builtin_plus));
  scope_add_mapping(root, stringpool_add("-"), create_builtin(builtin_sub));
  scope_add_mapping(root, stringpool_add("*"), create_builtin(builtin_mult));
  scope_add_mapping(root, stringpool_add("/"), create_builtin(builtin_div));

  // Comparison operators
  scope_add_mapping(root, stringpool_add("="), create_builtin(builtin_eq));
  scope_add_mapping(root, stringpool_add("<"), create_builtin(builtin_lt));
  scope_add_mapping(root, stringpool_add(">"), create_builtin(builtin_gt));

  // List manipulation
  scope_add_mapping(root, stringpool_add("LEN"), create_builtin(builtin_len));
  scope_add_mapping(root, stringpool_add("NTH"), create_builtin(builtin_nth));
  scope_add_mapping(root, stringpool_add("HEAD"), create_builtin(builtin_head));
  scope_add_mapping(root, stringpool_add("TAIL"), create_builtin(builtin_tail));
  scope_add_mapping(root, stringpool_add("LIST"), create_builtin(builtin_list));
  scope_add_mapping(root, stringpool_add("CONS"), create_builtin(builtin_cons));

  // String Manipulation
  scope_add_mapping(root, stringpool_add("STRLEN"), create_builtin(builtin_strlen));
  scope_add_mapping(root, stringpool_add("STRCMP"), create_builtin(builtin_strcmp));
  scope_add_mapping(root, stringpool_add("STRCAT"), create_builtin(builtin_strcat));
  scope_add_mapping(root, stringpool_add("SUBSTR"), create_builtin(builtin_substr));
  scope_add_mapping(root, stringpool_add("STRTOL"), create_builtin(builtin_strtol));

  // Load library functions.
  loadstring(root, (const char *)src_lib_lisp);

  return root;
}

bool use_panic_jmpbuf = false;
jmp_buf panic_jmpbuf;

void run_repl() {
  struct scope *root = create_root_scope();

  while (true) {
    use_panic_jmpbuf = true;
    setjmp(panic_jmpbuf);

    // Read line from standard in
    char *line = readline(">> ");

    // Evaluate expression
    struct expr *result = loadstring(root, line);

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
  GC_INIT();

  if (argc == 1) // No arguments means run REPL
    run_repl();

  // One argument means a file to load
  if (argc == 2) {
    scope *root = create_root_scope();
    loadfile(root, argv[1]);
  }
}
