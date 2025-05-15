(define
	(domain sokoban)
	(:requirements :adl)
	(:predicates
		(paczka ?x)
		(cel ?x)
        (poziomo ?x ?y)
        (pionowo ?x ?y)
        (robot ?x)
	)
    (:action idz 
        :parameters (?skad ?dokad)
        :precondition
        (and
            (robot ?skad)
            (not(paczka ?dokad))
			(or
                (poziomo ?skad ?dokad)
                (pionowo ?skad ?dokad)
            )
		)
		:effect
		(and
			(not(robot ?skad))
            (robot ?dokad)
		)
    )
    (:action pchaj 
        :parameters (?skad ?dokad)
        :precondition
        (and
			(robot ?skad)
            (paczka ?dokad)
            (exists (?nowe) 
                (or
                    (and
                        (poziomo ?skad ?dokad)
                        (poziomo ?dokad ?nowe)
                        (not(= ?skad ?nowe))
                        (not(paczka ?nowe))
                    )
                    (and
                        (pionowo ?skad ?dokad)
                        (pionowo ?dokad ?nowe)
                        (not(= ?skad ?nowe))
                        (not(paczka ?nowe))
                    )
                )
            )
		)
		:effect
		(and
			(not(robot ?skad))
            (robot ?dokad)
            (not(paczka ?dokad))
            (paczka ?nowe)
		)
    )
)
