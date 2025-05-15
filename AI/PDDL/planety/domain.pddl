(define
	(domain stars)
	(:requirements :adl)
    (:types gwiazda miejsce planeta przycisk dzwignia)
	(:predicates
		(human ?x - planeta)
		(rozwiaz ?x - planeta)
        (at ?b - przycisk ?x - planeta)
        (uloz ?x - gwiazda ?y - miejsce)
        (przejscie ?x ?y - planeta ?z - dzwignia)
        (mogeisc ?x ?y - planeta)
        (puste ?p - miejsce)
        (somsiad ?x ?y - miejsce)
        (przesunieta ?x - dzwignia)
        (zablokowana ?x - dzwignia)
        (nacisniety ?b - przycisk)
        (wajha ?d - dzwignia ?p - planeta)
        (blokuje ?b - przycisk ?d - dzwignia)
	)
    (:action przesungwiazde
        :parameters (?g - gwiazda ?skad ?dokad - miejsce ?p - planeta)
        :precondition
        (and
            (human ?p)
            (rozwiaz ?p)
            (uloz ?g ?skad)
            (somsiad ?skad ?dokad)
            (puste ?dokad)
        )
        :effect
        (and
            (not(uloz ?g ?skad))
            (puste ?skad)
            (uloz ?g ?dokad)
            (not (puste ?dokad))
        )
    )
    (:action nacisnijprzycisk
        :parameters (?b - przycisk ?p - planeta ?d - dzwignia)
        :precondition
        (and
            (human ?p)
            (at ?b ?p)
            (blokuje ?b ?d)
            ;(zablokowana ?d)
        )
        :effect
        (and
            (nacisniety ?b)
            (not (zablokowana ?d))
        )
    )
    (:action teleportuj
        :parameters (?skad ?dokad - planeta ?d - dzwignia)
        :precondition
        (and
            (human ?skad)
            (przejscie ?skad ?dokad ?d)
            (przesunieta ?d)
        )
        :effect
        (and
            (not (human ?skad))
            (human ?dokad)
            (when
                (not(mogeisc ?skad ?dokad))
                (not(przesunieta ?d))
            )
        )
    )
    (:action przesundzwignie
        :parameters (?d - dzwignia ?gdzie - planeta)
        :precondition
        (and
            (human ?gdzie)
            (not (zablokowana ?d))
            (not (przesunieta ?d))
            (wajha ?d ?gdzie)
        )
        :effect
        (and
            (przesunieta ?d)
        )
    )
)
