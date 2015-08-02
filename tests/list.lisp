(set a (list 2 3 4 "hello"))
(print (head a))
(print (len (tail a)))

(set b (cons 10 a))
(print (nth 0 b))
(print (nth 4 b))