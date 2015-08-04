(defun print-wrapper args
  (print (len args) "arguments supplied.")
  (apply print args))
(print-wrapper 4 5 "hello" FALSE)