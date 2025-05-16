-- 1.
CREATE OR REPLACE PROCEDURE NowyPracownik
    (pNazwisko IN VARCHAR,
     pZespol IN VARCHAR,
     pSzef IN VARCHAR,
     pPlaca IN NUMBER,
     pZatrudniony IN DATE DEFAULT CURRENT_DATE,
     pEtat IN VARCHAR DEFAULT 'STAZYSTA') IS

    vIDZesp Zespoly.id_zesp%TYPE;
    vIDSzef Pracownicy.id_prac%TYPE;
BEGIN
    SELECT id_zesp INTO vIDZesp FROM zespoly WHERE nazwa = pZespol;
    SELECT id_prac INTO vIDSzef FROM pracownicy WHERE nazwisko = pSzef;

    INSERT INTO Pracownicy VALUES (300, pNazwisko, pEtat, vIDSzef, pZatrudniony, pPlaca, 0, vIDZesp);
END NowyPracownik;
/

EXECUTE NowyPracownik('Dyndalski', 'ALGORYTMY', 'BLAZEWICZ', 250);
/

-- 2.
CREATE OR REPLACE FUNCTION PlacaNetto
    (pBrutto IN Pracownicy.placa_pod%TYPE,
     pPodatek IN NUMBER DEFAULT 20)
     RETURN NUMBER IS vNetto NUMBER;
BEGIN
    vNetto := pBrutto * (1 - pPodatek / 100);
    RETURN vNetto;
END;
/

-- 3.
CREATE OR REPLACE FUNCTION Silnia
    (pN IN NATURAL)
    RETURN NATURAL IS
    rSilnia NATURAL;
    vLicznik NATURAL := 1;
BEGIN
    rSilnia := 1;
    LOOP
        IF vLicznik >= pN THEN
            EXIT;
        END IF;
        vLicznik := vLicznik + 1;
        rSilnia := rSilnia * vLicznik;
    END LOOP;
    RETURN rSilnia;
END;
/

-- 4.
CREATE OR REPLACE FUNCTION SilniaRek
    (pN IN NATURAL)
    RETURN NATURAL IS
    rSilnia NATURAL;
    vN NATURAL := 1;
BEGIN
    vN := pN;
    IF pN <= 1 THEN
        RETURN 1;
    ELSE
        vN := vN * SilniaRek(vN - 1);
        RETURN vN;
    END IF;
END;
/

-- 5.
CREATE OR REPLACE FUNCTION IleLat
    (pData IN DATE)
    RETURN NATURAL IS Ile NATURAL;
BEGIN
    Ile := EXTRACT(YEAR FROM CURRENT_DATE) - EXTRACT(YEAR FROM pData);
    RETURN Ile;
END;
/

-- 6.
CREATE OR REPLACE PACKAGE Konwersja IS
    FUNCTION Cels_To_Fahr (pTC NUMBER) RETURN NUMBER;
    FUNCTION Fahr_To_Cels (pTF NUMBER) RETURN NUMBER;
END Konwersja;
/

CREATE OR REPLACE PACKAGE BODY Konwersja IS
    FUNCTION Cels_To_Fahr (pTC NUMBER) RETURN NUMBER IS
        rResult NUMBER;
    BEGIN
        rResult := 9 / 5 * pTC + 32;
        RETURN rResult;
    END Cels_To_Fahr;

    FUNCTION Fahr_To_Cels (pTF NUMBER) RETURN NUMBER IS
        rResult NUMBER;
    BEGIN
        rResult := 5 / 9 * (pTF - 32);
        RETURN rResult;
    END Fahr_To_Cels;
END Konwersja;
/

-- 7.
CREATE OR REPLACE PACKAGE Zmienne IS
    vLicznik INTEGER := 0;
    PROCEDURE ZwiekszLicznik;
    PROCEDURE ZmniejszLicznik;
    FUNCTION PokazLicznik RETURN INTEGER;
END Zmienne;
/

CREATE OR REPLACE PACKAGE BODY Zmienne IS
    PROCEDURE ZwiekszLicznik IS
    BEGIN
        vLicznik := vLicznik + 1;
        DBMS_OUTPUT.PUT_LINE('Zwiększono');
    END ZwiekszLicznik;

    PROCEDURE ZmniejszLicznik IS
    BEGIN
        vLicznik := vLicznik - 1;
        DBMS_OUTPUT.PUT_LINE('Zmniejszono');
    END ZmniejszLicznik;

    FUNCTION PokazLicznik RETURN INTEGER IS
    BEGIN
        RETURN vLicznik;
    END PokazLicznik;

BEGIN
    vLicznik := 1;
    DBMS_OUTPUT.PUT_LINE('Zainicjalizowano');
END Zmienne;
/

-- 8.
CREATE OR REPLACE PACKAGE IntZespoly IS
    PROCEDURE DodajZespol (pID NATURAL, pNazwa VARCHAR, pAdres VARCHAR);
    PROCEDURE UsunZespolPoID (pID NATURAL);
    PROCEDURE UsunZespolPoNazwie (pNazwa VARCHAR);
    PROCEDURE EdytujZespol (pID NATURAL, pNazwa VARCHAR, pAdres VARCHAR);
    FUNCTION PokazID (pNazwa VARCHAR) RETURN NATURAL;
    FUNCTION PokazNazwa (pID NATURAL) RETURN VARCHAR;
    FUNCTION PokazAdres (pID NATURAL) RETURN VARCHAR;
END IntZespoly;
/

CREATE OR REPLACE PACKAGE BODY IntZespoly IS
    PROCEDURE DodajZespol (pID NATURAL, pNazwa VARCHAR, pAdres VARCHAR) IS
    BEGIN
        INSERT INTO Zespoly VALUES (pID, pNazwa, pAdres);
    END DodajZespol;

    PROCEDURE UsunZespolPoID (pID NATURAL) IS
    BEGIN
        DELETE FROM Zespoly WHERE id_zesp = pID;
    END UsunZespolPoID;

    PROCEDURE UsunZespolPoNazwie (pNazwa VARCHAR) IS
    BEGIN
        DELETE FROM Zespoly WHERE nazwa = pNazwa;
    END UsunZespolPoNazwie;

    PROCEDURE EdytujZespol (pID NATURAL, pNazwa VARCHAR, pAdres VARCHAR) IS
    BEGIN
        UPDATE Zespoly SET nazwa = pNazwa WHERE id_zesp = pID;
        UPDATE Zespoly SET adres = pAdres WHERE id_zesp = pID;
    END EdytujZespol;

    FUNCTION PokazID (pNazwa VARCHAR) RETURN NATURAL IS
        vResult NATURAL;
    BEGIN
        SELECT id_zesp INTO vResult FROM zespoly WHERE nazwa = pNazwa;
        RETURN vResult;
    END PokazID;

    FUNCTION PokazNazwa (pID NATURAL) RETURN VARCHAR IS
        vResult VARCHAR(100);
    BEGIN
        SELECT nazwa INTO vResult FROM zespoly WHERE id_zesp = pID;
        RETURN vResult;
    END PokazNazwa;

    FUNCTION PokazAdres (pID NATURAL) RETURN VARCHAR IS
        vResult VARCHAR(100);
    BEGIN
        SELECT adres INTO vResult FROM zespoly WHERE id_zesp = pID;
        RETURN vResult;
    END PokazAdres;
END IntZespoly;
/

-- 9.
SELECT object_name, object_type, status
FROM User_Objects
WHERE object_type IN ('PROCEDURE', 'FUNCTION', 'PACKAGE');

-- 10.
DROP FUNCTION Silnia;
DROP FUNCTION SilniaRek;
DROP FUNCTION IleLat;

-- 11.
DROP PACKAGE Konwersja;
