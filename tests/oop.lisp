;; Inspired by Scheming With Objects
(defun NEW-POINT (x y)
  ;; Getters
  (defun get-x () x)
  (defun get-y () y)
  
  ;; Setters
  (defun set-x (newx)
    (set x newx))
  (defun set-y (newy)
    (set y newy))

  ;; Printer
  (defun disp ()
    (print "(" (self 'get-x)
      "," (self 'get-y) ")"))
  
  ;; Dispatcher function
  (defun self args
    (set message (head args))
    (set args (tail args))
    (case message
      ('get-x (apply get-x args))
      ('get-y (apply get-y args))
      ('set-x (apply set-x args))
      ('set-y (apply set-y args))
      ('disp (apply disp args))
    ))

  ;; Return dispatcher
  self)

(set a (NEW-POINT 0 0))
(a 'disp)
(a 'set-x 10)
(a 'set-y 20)
(a 'disp)

(set b (NEW-POINT 0 0))
(b 'disp)
(b 'set-x 102)
(b 'set-y 20123)

(a 'disp)
(b 'disp)