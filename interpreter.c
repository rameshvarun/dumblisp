#include "lexer.h"

#include "stdlib.h"
#include <readline/readline.h>
#include <readline/history.h>

#define REPL_LINE_MAX 4096

void run_repl() {
  while (true) {
    // Read line from standard in
    char *line = readline(">> ");

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
    create_lexing_context(&ctx, argv[1]);

    // For now simply prints out tokens
    while (true) {
      token_t tok;
      get_next_token(&ctx, &tok);

      switch (tok.type) {
      case STRING_TOKEN:
      case SYMBOL_TOKEN:
        printf("%s at %s:%d - %s\n", get_token_type_name(tok.type), ctx.filename, tok.lineno,
               tok.data.string_value);
        break;
      case INT_TOKEN:
        printf("%s at %s:%d - %d\n", get_token_type_name(tok.type), ctx.filename, tok.lineno,
               tok.data.int_value);
        break;
      default:
        printf("%s at %s:%d\n", get_token_type_name(tok.type), ctx.filename, tok.lineno);
      }

      if (tok.type == EOF_TOKEN)
        break;
    }
  }
}
