-- 1
SELECT NAZWISKO, P.ID_ZESP, NAZWA
FROM PRACOWNICY P
LEFT OUTER JOIN ZESPOLY Z ON P.ID_ZESP = Z.ID_ZESP
ORDER BY NAZWISKO;

-- 2
SELECT NAZWA, Z.ID_ZESP, COALESCE(NAZWISKO, 'BRAK PRACOWNIKOW') AS PRACOWNIK
FROM PRACOWNICY P
RIGHT OUTER JOIN ZESPOLY Z ON P.ID_ZESP = Z.ID_ZESP
ORDER BY NAZWA, NAZWISKO;

-- 3 (duplicate of 2)
SELECT NAZWA, Z.ID_ZESP, COALESCE(NAZWISKO, 'BRAK PRACOWNIKOW') AS PRACOWNIK
FROM PRACOWNICY P
RIGHT OUTER JOIN ZESPOLY Z ON P.ID_ZESP = Z.ID_ZESP
ORDER BY NAZWA, NAZWISKO;

-- 4
SELECT NAZWA AS ZESPOL,
       COUNT(NAZWISKO) AS LICZBA,
       SUM(COALESCE(PLACA_POD, 0)) AS SUMA_PLAC
FROM ZESPOLY Z, PRACOWNICY P
WHERE P.ID_ZESP (+) = Z.ID_ZESP
GROUP BY NAZWA
ORDER BY NAZWA;

-- 5
SELECT NAZWA
FROM ZESPOLY Z, PRACOWNICY P
WHERE P.ID_ZESP (+) = Z.ID_ZESP
GROUP BY NAZWA
HAVING COUNT(NAZWISKO) = 0
ORDER BY NAZWA;

-- 6
SELECT P.NAZWISKO AS PRACOWNIK, P.ID_PRAC, S.NAZWISKO AS SZEF, P.ID_SZEFA
FROM PRACOWNICY P
LEFT OUTER JOIN PRACOWNICY S ON P.ID_SZEFA = S.ID_PRAC
ORDER BY PRACOWNIK;

-- 7
SELECT S.NAZWISKO AS PRACOWNIK, COUNT(P.NAZWISKO) AS LICZBA_PODWLADNYCH
FROM PRACOWNICY P
RIGHT OUTER JOIN PRACOWNICY S ON P.ID_SZEFA = S.ID_PRAC
GROUP BY S.NAZWISKO
ORDER BY S.NAZWISKO;

-- 8
SELECT P.NAZWISKO, P.ETAT, P.PLACA_POD, Z.NAZWA, S.NAZWISKO AS SZEF
FROM PRACOWNICY P, PRACOWNICY S, ZESPOLY Z
WHERE S.ID_PRAC (+) = P.ID_SZEFA
  AND Z.ID_ZESP (+) = P.ID_ZESP
ORDER BY NAZWISKO;

-- 9
SELECT NAZWISKO, NAZWA
FROM PRACOWNICY
CROSS JOIN ZESPOLY
ORDER BY NAZWISKO;

-- 10
SELECT COUNT(*)
FROM (PRACOWNICY CROSS JOIN ZESPOLY) CROSS JOIN ETATY;

-- 11
SELECT ETAT
FROM PRACOWNICY
WHERE ZATRUDNIONY BETWEEN DATE '1992-01-01' AND DATE '1992-12-31'
INTERSECT
SELECT ETAT
FROM PRACOWNICY
WHERE ZATRUDNIONY BETWEEN DATE '1993-01-01' AND DATE '1993-12-31'
ORDER BY ETAT;

-- 12
SELECT ID_ZESP
FROM ZESPOLY
MINUS
SELECT ID_ZESP
FROM PRACOWNICY
ORDER BY ID_ZESP;

-- 13
SELECT ID_ZESP, NAZWA
FROM ZESPOLY
MINUS
SELECT P.ID_ZESP, NAZWA
FROM PRACOWNICY P
INNER JOIN ZESPOLY Z ON P.ID_ZESP = Z.ID_ZESP
ORDER BY ID_ZESP;

-- 14
SELECT NAZWISKO, PLACA_POD, 'Ponizej 480 zlotych' AS PROG
FROM PRACOWNICY
WHERE PLACA_POD < 480

UNION ALL

SELECT NAZWISKO, PLACA_POD, 'Dokladnie 480 zlotych' AS PROG
FROM PRACOWNICY
WHERE PLACA_POD = 480

UNION ALL

SELECT NAZWISKO, PLACA_POD, 'Powyzej 480 zlotych' AS PROG
FROM PRACOWNICY
WHERE PLACA_POD > 480

ORDER BY PLACA_POD;
