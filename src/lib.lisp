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

;; Return a range of numbers
(defun range (start end step)
  (if (>= start end)
    ()
    (cons start (range (+ start step) end step))
    ))

;; Execute a set of statements and return the value of the last one.
(defmacro progn args
  (cons 'let (cons '() args)))

;; A switch-case statement
(defmacro case args
  ;; The first argument is the varaible to switch over,
  ;; while the next arguments are the branches
  (set var (head args))
  (set branches (tail args))

  (if (= (len branches) 0)
    ;; In the base case, we have no branches, so
    ;; we simply return an empty list.
    (list)

    ;; Otherwise, we do an if else.
    (progn
      (set branch (head branches)) ;; The next branch to check.
      (set val (head branch)) ;; What to compare the variable to.
      (set stmnt (nth 1 branch)) ;; What to execute if var == val
      (list 'if (list '= var val)
        stmnt ;; If var == val, execute stmnt
        (cons 'case (cons var (tail branches))) ;; Otherwise, continue checking branches
      )
    )
  )
)
