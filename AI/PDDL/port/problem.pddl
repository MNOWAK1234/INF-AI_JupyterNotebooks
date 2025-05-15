(define (problem paczki)
    (:domain port)
    (:objects
        auto - samochod
        pole1 pole2 pole3 pole4 - miejsce
        one1 two1 three1 three2 four1 - zuraw
        a b c d e f paletaA1 paletaA2 paletaB1 paletaC1 paletaC2 paletaD1 - paczka
    )
    (:init
        (somsiad pole1 pole2)
        (somsiad pole2 pole1)
        (somsiad pole2 pole3)
        (somsiad pole3 pole2)
        (somsiad pole3 pole4)
        (somsiad pole4 pole3)
        (somsiad pole4 pole1)
        (somsiad pole1 pole4)

        (gdzie-zuraw one1 pole1)
        (gdzie-zuraw two1 pole2)
        (gdzie-zuraw three1 pole3)
        (gdzie-zuraw three2 pole3)
        (gdzie-zuraw four1 pole4)

        (na d paletaA1)
        (na a d)
        (na e paletaA2)
        (na b e)
        (na f paletaD1)
        (na c f)

        (miejsce-paczki a pole1)
        (miejsce-paczki b pole1)
        (miejsce-paczki c pole4)
        (miejsce-paczki d pole1)
        (miejsce-paczki e pole1)
        (miejsce-paczki f pole4)
        (miejsce-paczki paletaA1 pole1)
        (miejsce-paczki paletaA2 pole1)
        (miejsce-paczki paletaB1 pole2)
        (miejsce-paczki paletaC1 pole3)
        (miejsce-paczki paletaC2 pole3)
        (miejsce-paczki paletaD1 pole4)

        (na-gorze paletaB1)
        (na-gorze paletaC1)
        (na-gorze paletaC2)
        (na-gorze a)
        (na-gorze b)
        (na-gorze c)

        (robot auto pole2)
        (puste auto)

        (wolny one1)
        (wolny two1)
        (wolny three1)
        (wolny three2)
        (wolny four1)
    )
    (:goal
        (and
            (na a paletaD1)
            (na b a)
            (na c b)
            (na d c)
            (na e d)
            (na f e)
        )
    )
)
