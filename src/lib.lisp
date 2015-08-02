;; Increment Macro
(defmacro ++ (x)
  (list 'set x (list '+ x 1)))