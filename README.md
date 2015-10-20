# Dumblisp [![Build Status](https://travis-ci.org/rameshvarun/dumblisp.svg)](https://travis-ci.org/rameshvarun/dumblisp)

A dumb lisp interpreter, made for practice.

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->


- [Features](#features)
- [Compiling](#compiling)
- [Built-in Functions](#built-in-functions)
  - [I/O Utilities](#io-utilities)
    - [(print a b ...)](#print-a-b-)
  - [Function / Macro Creation](#function--macro-creation)
    - [(defun name (arg-a arg-b ...) stmnt-a stmnt-b ...)](#defun-name-arg-a-arg-b--stmnt-a-stmnt-b-)
    - [(lambda (arg-a arg-b ...) stmnt-a stmnt-b ...)](#lambda-arg-a-arg-b--stmnt-a-stmnt-b-)
    - [(defmacro name (arg-a arg-b ...) stmnt-a stmnt-b ...)](#defmacro-name-arg-a-arg-b--stmnt-a-stmnt-b-)
    - [Variable arguments](#variable-arguments)
  - [Scope / Variable Manipulation](#scope--variable-manipulation)
    - [(let ((var-a val-a) (var-b val-b) ...) expr-a expr-b ...)](#let-var-a-val-a-var-b-val-b--expr-a-expr-b-)
    - [(set symbol value)](#set-symbol-value)
  - [Control Flow](#control-flow)
    - [(if cond true-expr false-expr)](#if-cond-true-expr-false-expr)
    - [(while cond stmt-a stmnt-b ...)](#while-cond-stmt-a-stmnt-b-)
    - [(case var (value-a stmnts...) (value-b stmnts...))](#case-var-value-a-stmnts-value-b-stmnts)
  - [Other Builtins](#other-builtins)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Features
- A libreadline-based REPL
- [Create `lambda` functions with closures](tests/closures.lisp)
  - [Emulate OOP](tests/oop.lisp)
- [Create macros](tests/macro.lisp)
- Garbage collection using the Boehm GC
- It's dumb

## Compiling
```bash
# Install dependencies.
sudo apt-get install libreadline-dev libgc-dev # On Linux
brew install bdw-gc # On Mac OSX

# Get the source.
git clone https://github.com/rameshvarun/dumblisp.git
cd dumblisp
make && make test
```

## Built-in Functions
### I/O Utilities
#### (print a b ...)
`print` can be used to write to stdout. Print takes at least 1 argument.
```lisp
(print "Hello World" 3)
```

### Function / Macro Creation
#### (defun name (arg-a arg-b ...) stmnt-a stmnt-b ...)
```lisp
(defun fib (n)
  (if (or (= n 0) (= n 1))
    1
    (+ (fib (- n 1)) (fib (- n 2)))))
```

#### (lambda (arg-a arg-b ...) stmnt-a stmnt-b ...)
```lisp
(defun create-counter ()
  (print "Creating new counter...")
  (let ((i 0)) ; Start counter at 0
    (lambda ()
      (set i (+ i 1))
      i)))
```

#### (defmacro name (arg-a arg-b ...) stmnt-a stmnt-b ...)
Macros essentially act like functions that take in their arguments as literal expressions, returning an expression that is then immediately evaluated.
```lisp
(defmacro ++ (x)
  (list 'set x (list '+ x 1)))
```

#### Variable arguments
For both macros and functions, you can take in variable arguments by simply providing a symbol instead of an argument list.
```lisp
(defun print-wrapper args
  (print (len args) "arguments supplied.")
  (apply print args))
```

### Scope / Variable Manipulation
#### (let ((var-a val-a) (var-b val-b) ...) expr-a expr-b ...)
`let` creates a new scope, binds identifiers to values in that scope (in order), and evaluates expressions. `plet` binds all of the variables in parallel.

#### (set symbol value)
Set sets the value of the symbol, obeying lexical scope. If no binding exists in the scope stack, then set automatically creates it in the closes scope.

### Control Flow
#### (if cond true-expr false-expr)
The `false-expr` is optional.

#### (while cond stmt-a stmnt-b ...)
A basic while loop.

#### (case var (value-a stmnts...) (value-b stmnts...))
The case statement itself is implemented as a macro on top of the `if` builtin.

### Other Builtins
- Arithmatic Operators (`+`, `-`, `/`, `*`)
- Comparison Operators (`=`, `<`, `>`, `<=`, `>=`)
- Boolean Operators (`OR`, `AND`, `NOT`)
- List Manipulation Functions (`LIST`, `LEN`, `HEAD`, `TAIL`, `NTH`, `CONS`)
- String Manipulation Functions (`STRLEN`, `STRCMP`, `STRCAT`, `SUBSTR`)
- `TYPEOF` function
- `PROGN` macro
