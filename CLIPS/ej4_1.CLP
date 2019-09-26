defglobal ?*numero* = 15
 (defglobal ?*n* = 0)




(defrule juego
=>
(assert (lee)))



(defrule lee
?h <- (lee)
=>
(retract ?h)
(printout t "Escribe un numero: ")
(assert (numero (read))))
(defrule bajo
?h <- (numero ?n&:(< ?n ?*numero*))
=>
(retract ?h)
(printout t ?n " es menor" crlf)
(assert (lee)))
(defrule alto
?h <- (numero ?n&:(> ?n ?*numero*))
=>
(retract ?h)
(printout t ?n " es mayor" crlf)
(assert (lee)))
(defrule exacto
?h <- (numero ?n&:(= ?n ?*numero*))
=>
(retract ?h)
(printout t ?n " es correcto" crlf))
