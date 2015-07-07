(defun fib (n)
  (if (or (= n 0) (= n 1))
    1
    (+ (fib (- n 1)) (fib (- n 2)))
    ))

(print "fib(0) =" (fib 0))
(print "fib(1) =" (fib 1))
(print "fib(2) =" (fib 2))
(print "fib(10) =" (fib 10))
