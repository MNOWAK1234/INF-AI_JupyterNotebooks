student(miko³aj,nowak,151813,9).
%1
parami([],[]).
parami([A,B|C],[[A,B]|X]):-parami(C,X).
%2
symetryczna([]).
symetryczna([_]).
symetryczna([A|B]):-append(NOWY,[A],B),symetryczna(NOWY).
%3
polowki([],[],[]).
polowki([A|B],LL,LP):-append(MID,[END],B),polowki(MID,LEWA,PRAWA),append([A],LEWA,LL),append(PRAWA,[END],LP).
%4
sumuj([],0).
sumuj([A|B],X):-sumuj(B,DALEJ),X is DALEJ+A.
%5
ile_dodatnich([],0).
ile_dodatnich([A|B],X):-ile_dodatnich(B,DALEJ),A>0,X is DALEJ+1.
ile_dodatnich([A|B],X):-ile_dodatnich(B,DALEJ),A=<0,X=DALEJ.
%6
plus_minus([],0).
plus_minus([A|B],X):-plus_minus(B,DALEJ),A='+',X is DALEJ+1.
plus_minus([A|B],X):-plus_minus(B,DALEJ),A='-',X is DALEJ-1.
%7
wstawiaj([],EL,1,NOWA):-NOWA=[EL].
wstawiaj([A|B],EL,1,NOWA):-NOWA=[EL|[A|B]].
wstawiaj([A|B],EL,LICZBA,NOWA):-MINUS is LICZBA-1,wstawiaj(B,EL,MINUS,DALEJ),NOWA=[A|DALEJ].
%8
usuwaj([_|B],1,L1):-L1=B.
usuwaj([_|B],ILE,L1):-MINUS is ILE-1,usuwaj(B,MINUS,L1).
%9
rozdziel([],_,[],[]).
rozdziel([A|B],X,L1,L2):-A<X,rozdziel(B,X,D1,L2),L1=[A|D1].
rozdziel([A|B],X,L1,L2):-A>=X,rozdziel(B,X,L1,D2),L2=[A|D2].
%10
powiel([],[],[]).
powiel([_|B],[0|M],L):-powiel(B,M,L).
powiel([A|B],[N|M],L):-N>0,ILE is N-1,powiel([A|B],[ILE|M],DALEJ),L=[A|DALEJ].
