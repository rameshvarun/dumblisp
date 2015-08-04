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

(defmacro square (x)
	(list '* x x))

(defmacro quit ()
	(list 'exit 0))

(defun apply args
  (set func (head args))
  (set fargs (nth 1 args))
  (eval (cons func fargs)))

(defun range (start end step)
  (if (>= start end)
    ()
    (cons start (range (+ start step) end step))
    ))