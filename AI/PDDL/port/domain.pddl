(define
	(domain port)
	(:requirements :adl)
    (:types samochod miejsce zuraw paczka)
	(:predicates
        (somsiad ?x ?y - miejsce)
		(gdzie-zuraw ?z - zuraw ?m - miejsce)
        (na ?x ?y - paczka)
        (miejsce-paczki ?p - paczka ?m - miejsce)
        (podniesiona ?p - paczka ?z - zuraw)
        (na-aucie ?p - paczka ?a - samochod)
        (na-gorze ?p - paczka)
        (robot ?a - samochod ?m - miejsce)
        (puste ?a - samochod)
        (wolny ?z - zuraw)
	)
    (:action wez-paczke-ze-sterty
        :parameters (?gora ?dol - paczka ?gdzie - miejsce ?z - zuraw)
        :precondition
        (and
            (na-gorze ?gora)
            (na ?gora ?dol) ;musi to byc zeby nie brac palety
            (miejsce-paczki ?gora ?gdzie)
            (miejsce-paczki ?dol ?gdzie)
            (gdzie-zuraw ?z ?gdzie)
            (wolny ?z)
        )
        :effect
        (and
            (not (na-gorze ?gora))
            (na-gorze ?dol)
            (not (na ?gora ?dol))
            (not (wolny ?z))
            (not (miejsce-paczki ?gora ?gdzie))
            (podniesiona ?gora ?z)
        )
    )
    (:action odloz-na-sterte
        :parameters (?gora ?dol - paczka ?gdzie - miejsce ?z - zuraw)
        :precondition
        (and
            (na-gorze ?dol)
            (miejsce-paczki ?dol ?gdzie)
            (gdzie-zuraw ?z ?gdzie)
            (not (wolny ?z))
            (podniesiona ?gora ?z)
        )
        :effect
        (and
            (na-gorze ?gora)
            (not (na-gorze ?dol))
            (na ?gora ?dol)
            (miejsce-paczki ?gora ?gdzie)
            (not (podniesiona ?gora ?z))
            (wolny ?z)
        )
    )
    (:action odloz-na-samochod
        :parameters (?p - paczka ?gdzie - miejsce ?z - zuraw ?a - samochod)
        :precondition
        (and
            (robot ?a ?gdzie)
            (not (wolny ?z))
            (podniesiona ?p ?z)
            (gdzie-zuraw ?z ?gdzie)
            (puste ?a)
        )
        :effect
        (and
            (not (podniesiona ?p ?z))
            (wolny ?z)
            (na-aucie ?p ?a)
            (not (puste ?a))
        )
    )
    (:action podnies-z-samochodu
        :parameters (?p - paczka ?gdzie - miejsce ?z - zuraw ?a - samochod)
        :precondition
        (and
            (robot ?a ?gdzie)
            (wolny ?z)
            (na-aucie ?p ?a)
            (gdzie-zuraw ?z ?gdzie)
            (not (puste ?a))
        )
        :effect
        (and
            (podniesiona ?p ?z)
            (not (wolny ?z))
            (not (na-aucie ?p ?a))
            (puste ?a)
        )
    )
    (:action jedz
        :parameters (?skad ?dokad - miejsce ?a - samochod)
        :precondition
        (and
            (robot ?a ?skad)
            (somsiad ?skad ?dokad)
        )
        :effect
        (and
            (not (robot ?a ?skad))
            (robot ?a ?dokad)
        )
    )
)
