(set hello "Hello")
(set world "World")

(set helloworld (strcat (strcat hello " ") world))

(print helloworld)
(print (strlen hello))
(print (substr helloworld 11 0))

(print (strcmp hello "Hello"))
(print (strcmp hello world))