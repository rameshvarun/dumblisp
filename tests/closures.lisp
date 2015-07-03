(defun create-counter ()
  (print "Creating new counter...")
  (let ((i 0)) ; Start counter at 0
    (lambda ()
      (set i (+ i 1))
      i
      )))

; Create a counter
(let ((a (create-counter)))
  (print (a))
  (print (a))
  (print (a)))

; Create a second counter
(let ((a (create-counter)))
  (print (a))
  (print (a))
  (print (a)))
