#pragma once

// Simple PANIC MACRO
#define PANIC(...)                                                                                 \
  {                                                                                                \
    fprintf(stderr, __VA_ARGS__);                                                                  \
    exit(1);                                                                                       \
  }

#define TRACE(...) printf(__VA_ARGS__);

// Maximum string and symbol name sizes (for simplicity)
#define MAX_LITERAL_SIZE 4096
#define MAX_INTEGER_LITERAL_SIZE 4096
#define MAX_SYMBOL_SIZE 4096