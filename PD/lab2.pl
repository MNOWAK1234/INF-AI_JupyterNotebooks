poziomy(odcinek(punkt(X1,Y),punkt(X2,Y))).
pionowy(odcinek(punkt(X,Y1),punkt(X,Y2))).
regularny(prostokąt(punkt(X1,Y1),punkt(X2,Y1),punkt(X2,Y2),punkt(X1,Y2)))

safe(state(F,W,G,C)) :- F=east, G=east.
safe(state(F,W,G,C)) :- F=west, G=west.
safe(state(F,W,G,C)) :- F=east, W=east, G=west, C=east.
safe(state(F,W,G,C)) :- F=west, W=west, G=east, C=west.
move(state(west, west, X3, X4), fwe, state(east, east, X3, X4)) :- safe (state (east, east, X3, X4)).
move(state(west, X2, X3, west), fce, state (east, X2, X3, east)) :- safe (state (east, X2, X3, east)).
move(state(west, X2, west, X4), fge, state(east, X2, east, X4)) - safe (state (east, X2, east, X4)).
move(state(west, X2, X3, X4), fe, state (east, X2, X3, X4)) - safe(state (east, X2, X3, X4)).
move(state(east, X2, X3, X4), fw, state(west, X2, X3, X4)) :- safe(state (west, X2, X3, X4)).
move(state(east, X2, east, X4), fgw, state (west, X2, west, X4)) :- safe (state (west, X2, west, X4)).
move(state(east, X2, X3, east), fcw, state (west, X2, X3, west)) :- safe(state (west, X2, X3, west)).
move(state(east, X2, X3, east), fcw, state(west, X2, X3, west)) :- safe(state(west, X2, X3, west)).

can_get(state(east, east, east, east)).
can_get(StateBefore) :- move(StateBefore, Move, StateAfter), can_get(stateAfter).