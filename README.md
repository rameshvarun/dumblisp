# Dumblisp [![Build Status](https://travis-ci.org/rameshvarun/dumblisp.svg)](https://travis-ci.org/rameshvarun/dumblisp)

A dumb lisp interpreter, made for practice.

## Features
- A libreadline-based REPL
- Create `lambda` functions with closures
- Create macros
- It's dumb

## Built-in Functions
### (print a b ...)
`print` can be used to write to stdout. Print take at least 1 argument.
```lisp
(print "Hello World" 3)
```

### (defun name (arg-a arg-b ...) stmnt-a stmnt-b ...)
```lisp
(defun fib (n)
  (if (or (= n 0) (= n 1))
    1
    (+ (fib (- n 1)) (fib (- n 2)))))
```

### (let ((var-a val-a) (var-b val-b) ...) expr-a expr-b ...)
`let` creates a new scope, binds identifiers to values in that scope, and evaluates expresions.

### (set symbol value)

### (lambda (arg-a arg-b ...) stmnt-a stmnt-b ...)
```lisp
(defun create-counter ()
  (print "Creating new counter...")
  (let ((i 0)) ; Start counter at 0
    (lambda ()
      (set i (+ i 1))
      i)))
```

### Arithmatic Operators (+, -, /, *)

### Comparison Operators (=, <, >)

## TODO
- Garbage collection
