(define (problem p1)
	(:domain world-of-blocks)
	(:objects a b c d e)
	(:init
		(clear c)
		(on-top c b)
		(on-top b a)
		(on-floor a)

        	(clear e)
		(on-top e d)
		(on-floor d)
	)
	(:goal
		(and
			(on-floor a)
            (on-floor b)
            (on-floor c)
            (on-floor d)
            (on-floor e)
		)
	)
)
