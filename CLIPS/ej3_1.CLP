(defrule leer-nombres
	(initial-fact)
  =>
    (open "C:\Users\adri_\Documents\nombres.txt" fichero "r")
    (bind ?data (readline fichero))
    (while (neq ?data EOF)
       (assert (nombre ?data))
       (bind ?data (readline fichero)))
    (close fichero)
)
