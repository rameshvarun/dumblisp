# Dumblisp [![Build Status](https://travis-ci.org/rameshvarun/dumblisp.svg)](https://travis-ci.org/rameshvarun/dumblisp)

A dumb lisp interpreter, made for practice.

## Features
- libreadline-based REPL
- It's dumb

## Built-in Functions
### print
`print` can be used to write to stdout. Print take at least 1 argument.
```lisp
(print "Hello World" 3)
```
### let
`let` creates a new scope, binds identifiers to values in that scope, and evaluates expresions.

### Arithmatic Operators (+, -, /, *)

## TODO
- Garbage collection
- Interning symbol strings
