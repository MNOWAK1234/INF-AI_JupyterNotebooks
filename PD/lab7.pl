student(miko³aj,nowak,151813,9).
%1
podzielniki(N,D,[]):-D=N.
podzielniki(N,D,A):-D<N,mod(N,D)=:=0,DALEJ is D+1,!,podzielniki(N,DALEJ,P),A=[D|P].
podzielniki(N,D,A):-D<N,mod(N,D)=\=0,DALEJ is D+1,!,podzielniki(N,DALEJ,A).
sumuj([],0).
sumuj([A|B],X):-sumuj(B,DALEJ),X is DALEJ+A.
doskonala(NUM):-podzielniki(NUM,1,DIV),sumuj(DIV,X),X=:=NUM.
%2
przekroj([],[],[]).
przekroj([],_,[]).
przekroj(_,[],[]).
przekroj([A|L1],[B|L2],S):-A=:=B,przekroj(L1,L2,RES),S=[A|RES].
przekroj([A|L1],[B|L2],S):-A<B,przekroj(L1,[B|L2],S).
przekroj([A|L1],[B|L2],S):-A>B,przekroj([A|L1],L2,S).
%3
glowyogony([],[],[]).
glowyogony([ROW1|ROW2],H,T):-ROW1=[A|B],glowyogony(ROW2,PRH,PRT),H=[A|PRH],T=[B|PRT].
slad([[A]],A).
slad([ROW1|ROW2],SUM):-ROW1=[A|_],glowyogony(ROW2,_,T),slad(T,DALEJ),SUM is DALEJ+A.
%4
rozszerzaj([A],[MIN,MAX]):-A=[MIN,MAX].
rozszerzaj([A|B],[L,P]):-A=[MIN,MAX],rozszerzaj(B,[PMIN,PMAX]),MIN<PMIN,PMAX<MAX,L=MIN,P=MAX.
rozszerzaj([A|B],[L,P]):-A=[MIN,MAX],rozszerzaj(B,[PMIN,PMAX]),MIN<PMIN,PMAX>=MAX,L=MIN,P=PMAX.
rozszerzaj([A|B],[L,P]):-A=[MIN,MAX],rozszerzaj(B,[PMIN,PMAX]),MIN>=PMIN,PMAX<MAX,L=PMIN,P=MAX.
rozszerzaj([A|B],[L,P]):-A=[MIN,MAX],rozszerzaj(B,[PMIN,PMAX]),MIN>=PMIN,PMAX>=MAX,L=PMIN,P=PMAX.
%5
ciagliczb(A,A,L):-L=[A].
ciagliczb(BEGIN,END,LIST):-BEGIN<END,DALEJ is BEGIN+1,ciagliczb(DALEJ,END,NEXT),LIST=[BEGIN|NEXT].
liczpodziel(N,N,0).
liczpodziel(N,D,A):-D<N,mod(N,D)=:=0,DALEJ is D+1,!,liczpodziel(N,DALEJ,P),A is P+1.
liczpodziel(N,D,A):-D<N,mod(N,D)=\=0,DALEJ is D+1,!,liczpodziel(N,DALEJ,A).
antyp([],_).
antyp([A|B],LP):-liczpodziel(A,1,DIVS),DIVS<LP,antyp(B,LP).
antypierwsza(NUM):-liczpodziel(NUM,1,LP),MINUSONE is NUM-1,ciagliczb(1,MINUSONE,LOWER),antyp(LOWER,LP).
%6
rozmien(LIST,NUM,P):-append(FRONT,[LAST],LIST),LAST>NUM,rozmien(FRONT,NUM,PREV),P=PREV,!.
rozmien(LIST,NUM,P):-append(_,[LAST],LIST),LAST=<NUM,NEW is NUM-LAST,NEW>0,rozmien(LIST,NEW,PREV),append(PREV,[LAST],P),!.
rozmien(LIST,NUM,P):-append(_,[LAST],LIST),LAST=<NUM,NEW is NUM-LAST,NEW=:=0,P=[LAST].
%7
wpolu([],_,[]).
wpolu([THIS|NEXT],P,ALL):-THIS=[A,B],P=[[C,D],[E,F]],A=<E,A>=C,B=<F,B>=D,!,wpolu(NEXT,P,PREV),ALL=[THIS|PREV].
wpolu([THIS|NEXT],P,ALL):-THIS=[A,_],P=[[C,_],[_,_]],A<C,!,wpolu(NEXT,P,PREV),ALL=PREV.
wpolu([THIS|NEXT],P,ALL):-THIS=[_,B],P=[[_,D],[_,_]],B<D,!,wpolu(NEXT,P,PREV),ALL=PREV.
wpolu([THIS|NEXT],P,ALL):-THIS=[A,_],P=[[_,_],[E,_]],A>E,!,wpolu(NEXT,P,PREV),ALL=PREV.
wpolu([THIS|NEXT],P,ALL):-THIS=[_,B],P=[[_,_],[_,F]],B>F,!,wpolu(NEXT,P,PREV),ALL=PREV.
%8
minX([[A,_]],A).
minX([THIS|NEXT],MIN):-minX(NEXT,PREV),THIS=[A,_],A<PREV,MIN=A.
minX([THIS|NEXT],MIN):-minX(NEXT,PREV),THIS=[A,_],A>=PREV,MIN=PREV.
maxX([[A,_]],A).
maxX([THIS|NEXT],MAX):-maxX(NEXT,PREV),THIS=[A,_],A>PREV,MAX=A.
maxX([THIS|NEXT],MAX):-maxX(NEXT,PREV),THIS=[A,_],A=<PREV,MAX=PREV.
minY([[_,B]],B).
minY([THIS|NEXT],MIN):-minY(NEXT,PREV),THIS=[_,B],B<PREV,MIN=B.
minY([THIS|NEXT],MIN):-minY(NEXT,PREV),THIS=[_,B],B>=PREV,MIN=PREV.
maxY([[_,B]],B).
maxY([THIS|NEXT],MAX):-maxY(NEXT,PREV),THIS=[_,B],B>PREV,MAX=B.
maxY([THIS|NEXT],MAX):-maxY(NEXT,PREV),THIS=[_,B],B=<PREV,MAX=PREV.
minpole(POINTS,P):-P=[[A,B],[C,D]],minX(POINTS,A),minY(POINTS,B),maxX(POINTS,C),maxY(POINTS,D).
%9
zamien_g([],[]).
zamien_g([X],[X]).
zamien_g([A|[B|REST]],[B|LIST]):-A>B,zamien_g([A|REST],LIST),!.
zamien_g([A|[B|REST]],[A|LIST]):-A=<B,zamien_g([B|REST],LIST).
zamien_d([],[]).
zamien_d([X],[X]).
zamien_d([A|[B|REST]],[B|LIST]):-A<B,zamien_d([A|REST],LIST),!.
zamien_d([A|[B|REST]],[A|LIST]):-A>=B,zamien_d([B|REST],LIST).
odwracaj([],[]).
odwracaj([A|B],REV):-odwracaj(B,PREV),append(PREV,[A],REV).
koktajl([], []).
koktajl([X], [X]).
koktajl(L,S):-zamien_g(L,L1),odwracaj(L1,[BIG|L2]),zamien_d(L2,L3),odwracaj(L3,[SMALL|L4]),koktajl(L4,S1),append([SMALL|S1],[BIG],S),!.
%10
ciagFib(N,F1,F2,[F1,F2]):-F1+F2>N.
ciagFib(N,F1,F2,FIB):-NEW is F1+F2,NEW=<N,ciagFib(N,F2,NEW,PREV),FIB=[F1|PREV].
rozklad(LIST,NUM,P):-append(FRONT,[LAST],LIST),LAST>NUM,rozklad(FRONT,NUM,PREV),P=PREV,!.
rozklad(LIST,NUM,P):-append(_,[LAST],LIST),LAST=<NUM,NEW is NUM-LAST,NEW>0,rozklad(LIST,NEW,PREV),append(PREV,[LAST],P),!.
rozklad(LIST,NUM,P):-append(_,[LAST],LIST),LAST=<NUM,NEW is NUM-LAST,NEW=:=0,P=[LAST].
rozklad_fib(N,L):-ciagFib(N,0,1,FIB),rozklad(FIB,N,L).
