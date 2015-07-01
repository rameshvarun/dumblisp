#ifndef SCOPE_H
#define SCOPE_H

// Represents a single binding of a variable to a value
struct mapping {
  struct mapping *next;

  const char *symbol;
  struct expr *value;
};

// A scope represents a set of bindings
struct scope {
  struct scope *parent;     // The parent scope in the heirarchy
  struct mapping *mappings; // The mappings in the current scope
};

// Traverse the scope heirarchy, returning the first expression that matches
// the symbol.
struct expr *scope_lookup(struct scope *scope, const char *symbol);

// Return an expression if a match for it exists in the current scope.
struct expr *scope_probe(struct scope *scope, const char *symbol);

// Allocates a scope with the given parent. Pass NULL if this is a root scope.
struct scope *scope_create(struct scope *parent);

// Adds a mapping to the given scope.
void scope_add_mapping(struct scope *scope, const char *symbol, struct expr *value);

#endif
