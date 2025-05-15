next(œwinoujœcie,ko³obrzeg).
next(œwinoujœcie, szczecin).
next(ko³obrzeg,ustka).
next(ustka, gdañsk).
next(gdañsk,toruñ).
next(gdañsk,olsztyn).
next(olsztyn,toruñ).
next(olsztyn,bia³ystok).
next(szczecin,bydgoszcz).
next(szczecin, gorzów-wlkp).
next(bydgoszcz,toruñ).
next(bydgoszcz,poznañ).
next(poznañ,toruñ).
next(gorzów-wlkp,poznañ).
next(gorzów-wlkp,zielona-góra).
next(zielona-góra,poznañ).
next(zielona-góra,wroc³aw).
next(toruñ,³ódŸ).
next(toruñ,warszawa).
next(bia³ystok, warszawa).
next(bia³ystok,lublin).
next(poznañ,³ódŸ).
next(warszawa,radom).
next(warszawa,³ódŸ).
next(radom,lublin).
next(radom,kielce).
next(wroc³aw,³ódŸ).
next(³ódŸ,radom).
next(³ódŸ,czêstochowa).
next(czêstochowa,kielce).
next(czêstochowa,katowice).
next(katowice,kraków).
next(kielce,kraków).
next(kielce,rzeszów).
next(lublin,kielce).
next(lublin,rzeszów).
next(kraków,rzeszów).

po³¹czenie(A,B):-next(A,B).
po³¹czenie(A,B):-next(A,C),po³¹czenie(C,B).

zjazd(A,B,C):-next(C,B),C=A.
zjazd(A,B,X):-next(E,X),zjazd(A,X,E),next(X,B).

trasa(A,B,obok(A,B)):-next(A,B).
trasa(A,B,obok(A,D)):-next(A,C),trasa(C,B,D).

trasa2(A,B,obok(A,B)):-next(A,B).
trasa2(A,B,obok(D,B)):-next(C,B),trasa2(A,C,D).

trasa3(A,B,[A | [B | []]]):-next(A,B).
trasa3(A,B,[A | D]):-next(A,C),trasa3(C,B,D).
