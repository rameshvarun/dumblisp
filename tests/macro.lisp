;; Macro with no arguments
(defmacro greet () (list 'print "Hello World!"))
(greet)
(greet)

;; Increment Macro
(defmacro incr (x)
  (list 'set x (list '+ x 1)))

(let ((i 0))
	(print i)
	(incr i)
	(incr i)
	(incr i)
	(print i))