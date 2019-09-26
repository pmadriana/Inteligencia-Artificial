(deftemplate Skin (slot skin))
(deftemplate Blood (slot blood))
(deftemplate Move (slot move))

; ------------------------
; ------INPUT-------------
; ------------------------


(defrule GetSkin
   (declare (salience 750))
   (not (animal ?))
   =>
   (printout t "Que tipo de piel tiene? (pelo/plumas/escamas/porosa): ")
   (bind ?response (read))
   (assert (Skin (skin ?response))))

(defrule GetBlood
   (declare (salience 700))
   (not (animal ?))
   =>
   (printout t "Que tipo de sangre tiene? (caliente/fria): ")
   (bind ?response (read))
   
   (assert (Blood (blood ?response))))

(defrule GetMove
   (declare (salience 780))
   (not (animal ?))
   =>
   (printout t "Como se mueve? (andando/volando): ")
   (bind ?response (read))
   (assert (Move (move ?response))))

;---------------------------
;--------RULES--------------
;---------------------------

(defrule Mamifero
	(declare (salience 600))
	(Skin (skin pelo))
	(Blood (blood caliente))
	(Move (move andando))
	=>
	(assert (animal mamifero))
	(printout t "Es mamifero!!" crlf))

(defrule Ave
	(declare (salience 680))
	(Skin (skin plumas))
	(Blood (blood caliente))
	(Move (move volando))
	=>
	(assert (animal ave))
	(printout t "Es un ave!!" crlf))

(defrule Reptil
	(declare (salience 749))
	(Skin (skin escamas))
	(Blood (blood fria))
	(Move (move na))
	=>
	(assert (animal reptil))
	(printout t "Es reptil!!" crlf))

(defrule Anfibio
	(declare (salience 499))
	(Skin (skin porosa))
	(Blood (blood fria))
	(Move (move na))
	=>
	(assert (animal anfibio))
	(printout t "Es anfibio!!" crlf))

