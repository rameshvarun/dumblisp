;; Continously allocate strings in a loop.

(set x "a")
(set y "b")
(set z "c")

(while (< (strlen x) 10000)
	(set x (strcat x "a"))
	(set y (strcat y "b"))
	(set z (strcat y "c")))

(print "Done.")