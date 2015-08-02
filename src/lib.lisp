;; Increment and Decrement Macros
(defmacro ++ (x)
  (list 'set x (list '+ x 1)))
(defmacro -- (x)
  (list 'set x (list '- x 1)))

;; Arithmatic + Assignment
(defmacro += (x y)
  (list 'set x (list '+ x y)))
(defmacro -= (x y)
  (list 'set x (list '- x y)))
(defmacro *= (x y)
  (list 'set x (list '* x y)))
(defmacro /= (x y)
  (list 'set x (list '/ x y)))

;; >= and <=
(defmacro <= (x y)
  (list 'or (list '< x y) (list '= x y)))
(defmacro >= (x y)
  (list 'or (list '> x y) (list '= x y)))

;; Negation
(defmacro ~ (x)
	(list '- 0 x))