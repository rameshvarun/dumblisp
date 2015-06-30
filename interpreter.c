#include "lexer.h"
#include "parser.h"

#include "stdlib.h"
#include <readline/readline.h>
#include <readline/history.h>

#define REPL_LINE_MAX 4096

void run_repl() {
  while (true) {
    // Read line from standard in
    char *line = readline(">> ");

    // Create a lexing context on the given line
    lexing_context ctx;
    create_string_lexer(&ctx, line);
    test_lexer(&ctx);

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
