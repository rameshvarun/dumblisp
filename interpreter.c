#include "lexer.h"

int main(int argc, char **argv) {
  if (argc == 2) {
    lexing_context ctx;
    create_lexing_context(&ctx, argv[1]);

    while (true) {
      token_t tok;
      get_next_token(&ctx, &tok);

      switch (tok.type) {
      case STRING:
      case SYMBOL:
        printf("%s at %s:%d - %s\n", get_token_type_name(tok.type),
               ctx.filename, tok.lineno, tok.data.string_value);
        break;
      default:
        printf("%s at %s:%d\n", get_token_type_name(tok.type), ctx.filename,
               tok.lineno);
      }

      if (tok.type == EOF_TOKEN)
        break;
    }
  }
}
