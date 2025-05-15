(define
	(domain game)
	(:requirements :adl)
    (:types klocek pole pomieszczenie kolor pilka)
	(:predicates
		(human ?x - pomieszczenie)
		(rozwiaz ?x - pomieszczenie)
        (postawiony ?x - klocek ?y - pole)
        (somsiad ?x ?y - pole)
        (przejscie ?k - kolor ?x ?y - pomieszczenie)
        (barwa ?x - pilka ?y - kolor)
        (ball ?x - pilka ?y - pomieszczenie)
        (moge-isc ?x ?y - pomieszczenie)
        (puste ?p - pole)
	)

    (:action przesun
        :parameters (?k - klocek ?skad ?dokad - pole ?miejsce - pomieszczenie)
        :precondition
        (and
            (human ?miejsce)
            (rozwiaz ?miejsce)
            (postawiony ?k ?skad)
            (somsiad ?skad ?dokad)
            (puste ?dokad)
        )
        :effect
        (and
            (not(postawiony ?k ?skad))
            (puste ?skad)
            (postawiony ?k ?dokad)
            (not (puste ?dokad))
        )
    )

    (:action idz
        :parameters (?dokad ?skad ?inne ?drugie - pomieszczenie ?col - kolor)
        :precondition
        (and
            (human ?skad)
            (przejscie ?col ?skad ?dokad)
            (przejscie ?col ?inne ?drugie)
            (moge-isc ?inne ?drugie)
        )
        :effect
		(and
			(not(human ?skad))
            (human ?dokad)
            (not(moge-isc ?inne ?drugie))
		)
    )

    (:action wez
        :parameters (?col - kolor ?room ?inny ?drugi - pomieszczenie ?p - pilka)
        :precondition
        (and
            (human ?room)
            (barwa ?p ?col)
            (ball ?p ?room)
            (not (moge-isc ?inny ?drugi))
            (przejscie ?col ?inny ?drugi)
            (exists (?room - pomieszczenie) (human ?room))
        )
        :effect
		(and
			(not(ball ?p ?room))
            (moge-isc ?inny ?drugi)
		)
    )

)
