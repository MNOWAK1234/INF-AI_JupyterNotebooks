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
			(on-top d b)
      (exists (?paczka) (on-top b ?paczka))
		)
	)
)
