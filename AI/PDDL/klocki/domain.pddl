(define
	(domain world-of-blocks)
	(:requirements :adl)
	(:predicates
		(on-top ?x ?y)
		(on-floor ?x)
		(clear ?x)
        (hold ?x)
	)
    (:action podnies-z-paczki 
        :parameters (?gora ?dol)
        :precondition
        (and
			(not (exists (?klocek) (hold ?klocek)))
            (clear ?gora)
			(on-top ?gora ?dol)
		)
		:effect
		(and
			(not (on-top ?gora ?dol))
			(hold ?gora)
			(clear ?dol)
            (not (clear ?gora))
		)
    )

    (:action podnies-z-podlogi 
        :parameters (?klocek)
        :precondition
        (and
			(not (exists (?inny) (hold ?inny)))
            (clear ?klocek)
            (on-floor ?klocek)
		)
		:effect
		(and
			(not (on-floor ?klocek))
			(hold ?klocek)
            (not (clear ?klocek))
		)
    )


    (:action opusc-na-podloge
        :parameters (?klocek)
        :precondition
        (and
			(hold ?klocek)
		)
		:effect
		(and
			(on-floor ?klocek)
			(not(hold ?klocek))
            (clear ?klocek)
		)
    )


    (:action opusc-na-paczke
        :parameters (?dolny ?gorny)
        :precondition
        (and
			(hold ?gorny)
            (clear ?dolny)
		)
		:effect
		(and
			(not(clear ?dolny))
			(not(hold ?gorny))
            (on-top ?gorny ?dolny)
            (clear ?gorny)
		)
    )

)
