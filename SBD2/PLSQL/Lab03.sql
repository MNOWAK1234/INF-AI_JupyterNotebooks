-- 1.
DECLARE
   CURSOR c_asystenci IS
      SELECT Nazwisko, Zatrudniony
      FROM Pracownicy
      WHERE Etat = 'ASYSTENT';

   v_nazwisko Pracownicy.Nazwisko%TYPE;
   v_zatrudniony Pracownicy.Zatrudniony%TYPE;
BEGIN
   OPEN c_asystenci;
   LOOP
      FETCH c_asystenci INTO v_nazwisko, v_zatrudniony;
      EXIT WHEN c_asystenci%NOTFOUND;
      DBMS_OUTPUT.PUT_LINE(v_nazwisko || ' pracuje od ' || v_zatrudniony);
   END LOOP;
   CLOSE c_asystenci;
END;
/

-- 2.
DECLARE
   CURSOR c_top_earning IS
      SELECT Nazwisko, Placa_Pod
      FROM Pracownicy
      ORDER BY Placa_Pod DESC;

   v_nazwisko Pracownicy.Nazwisko%TYPE;
   v_placa Pracownicy.Placa_Pod%TYPE;
   v_rowcount NUMBER := 0;
BEGIN
   OPEN c_top_earning;
   LOOP
      FETCH c_top_earning INTO v_nazwisko, v_placa;
      EXIT WHEN c_top_earning%NOTFOUND OR v_rowcount >= 3;

      v_rowcount := v_rowcount + 1;
      DBMS_OUTPUT.PUT_LINE(v_rowcount || ' : ' || v_nazwisko);
   END LOOP;
   CLOSE c_top_earning;
END;
/

-- 3.
DECLARE
   CURSOR c_zwieksz IS
      SELECT ID_PRAC, Placa_Pod
      FROM Pracownicy
      WHERE TO_CHAR(Zatrudniony, 'D') = 2; -- Poniedziałek (numer 2)

   v_id_prac Pracownicy.ID_PRAC%TYPE;
   v_placa_pod Pracownicy.Placa_Pod%TYPE;
BEGIN
   FOR r_zwieksz IN c_zwieksz LOOP
      v_id_prac := r_zwieksz.ID_PRAC;
      v_placa_pod := r_zwieksz.Placa_Pod;

      -- Zwiększ płacę podstawową o 20%
      v_placa_pod := v_placa_pod * 1.20;

      -- Zaktualizuj płacę pracownika
      UPDATE Pracownicy
      SET Placa_Pod = v_placa_pod
      WHERE ID_PRAC = v_id_prac;
   END LOOP;

   COMMIT;
END;
/

-- 4.
DECLARE
    CURSOR cPracownicy IS
        SELECT *
        FROM Pracownicy JOIN Zespoly USING(id_zesp)
        FOR UPDATE OF nazwisko;
BEGIN
    FOR vPracownik IN cPracownicy LOOP
        IF vPracownik.nazwa = 'ALGORYTMY' THEN
            UPDATE Pracownicy
            SET placa_dod = COALESCE(placa_dod,0) + 100
            WHERE CURRENT OF cPracownicy;
        ELSIF vPracownik.nazwa = 'ADMINISTRACJA' THEN
            UPDATE Pracownicy
            SET placa_dod = COALESCE(placa_dod,0) + 150
            WHERE CURRENT OF cPracownicy;
        ELSIF vPracownik.etat = 'STAZYSTA' THEN
            DELETE FROM Pracownicy
            WHERE CURRENT OF cPracownicy;
        END IF;
    END LOOP;
END;
/

-- 5.
CREATE OR REPLACE PROCEDURE PokazPracownikowEtatu (pEtat IN VARCHAR) IS
BEGIN
    DECLARE
    CURSOR cPracownicy (pEtat VARCHAR) IS
        SELECT *
        FROM Pracownicy
        WHERE etat = pEtat;
    BEGIN
        FOR vPracownik IN cPracownicy(pEtat) LOOP
            DBMS_OUTPUT.PUT_LINE(vPracownik.nazwisko);
        END LOOP;
    END;
END PokazPracownikowEtatu;
/

-- 6.
CREATE OR REPLACE PROCEDURE RaportKadrowy IS
BEGIN
    DECLARE
    CURSOR cEtaty IS
        SELECT *
        FROM Etaty;

    CURSOR cPracownicy (pEtat VARCHAR) IS
        SELECT *
        FROM Pracownicy
        WHERE etat = pEtat;

    vSuma NUMBER;
    vPensja NUMBER;
    vPracownik Pracownicy%ROWTYPE;
    BEGIN
        FOR vEtat IN cEtaty LOOP
            DBMS_OUTPUT.PUT_LINE('Etat: ' || vEtat.nazwa);
            DBMS_OUTPUT.PUT_LINE('------------------------------');

            OPEN cPracownicy(vEtat.nazwa);
            vSuma := 0;

            LOOP
                FETCH cPracownicy INTO vPracownik;
                EXIT WHEN cPracownicy%NOTFOUND;

                vPensja := vPracownik.placa_pod + COALESCE(vPracownik.placa_dod,0);
                DBMS_OUTPUT.PUT_LINE(cPracownicy%ROWCOUNT || '. ' || vPracownik.nazwisko || ', pensja: ' || vPensja);
                vSuma := vSuma + vPensja;
            END LOOP;

            DBMS_OUTPUT.PUT_LINE('Liczba pracowników: ' || cPracownicy%ROWCOUNT);
            IF cPracownicy%ROWCOUNT = 0 THEN
                DBMS_OUTPUT.PUT_LINE('Średnia pensja: brak');
            ELSE 
                DBMS_OUTPUT.PUT_LINE('Średnia pensja: ' || vSuma/cPracownicy%ROWCOUNT);
            END IF;

            CLOSE cPracownicy;
            DBMS_OUTPUT.PUT_LINE('');
        END LOOP;
    END;
END RaportKadrowy;
/

-- 7.
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
        IF SQL%NOTFOUND THEN
            DBMS_OUTPUT.PUT_LINE ('Nie udało się dodać zespołu');
        END IF;
    END DodajZespol;

    PROCEDURE UsunZespolPoID (pID NATURAL) IS
    BEGIN
        DELETE FROM Zespoly WHERE id_zesp = pID;
        IF SQL%NOTFOUND THEN
            DBMS_OUTPUT.PUT_LINE ('Nie udało się usunąć zespołu');
        END IF;
    END UsunZespolPoID;

    PROCEDURE UsunZespolPoNazwie (pNazwa VARCHAR) IS
    BEGIN
        DELETE FROM Zespoly WHERE nazwa = pNazwa;
        IF SQL%NOTFOUND THEN
            DBMS_OUTPUT.PUT_LINE ('Nie udało się usunąć zespołu');
        END IF;
    END UsunZespolPoNazwie;

    PROCEDURE EdytujZespol (pID NATURAL, pNazwa VARCHAR, pAdres VARCHAR) IS
    BEGIN
        UPDATE Zespoly SET nazwa = pNazwa WHERE id_zesp = pID;
        UPDATE Zespoly SET adres = pAdres WHERE id_zesp = pID;
        IF SQL%NOTFOUND THEN
            DBMS_OUTPUT.PUT_LINE ('Nie udało się edytować zespołu');
        END IF;
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

-- 8.
CREATE OR REPLACE PACKAGE IntZespoly IS
    PROCEDURE DodajZespol (pID NATURAL, pNazwa VARCHAR, pAdres VARCHAR);
    PROCEDURE UsunZespolPoID (pID NATURAL);
    PROCEDURE UsunZespolPoNazwie (pNazwa VARCHAR);
    PROCEDURE EdytujZespol (pID NATURAL, pNazwa VARCHAR, pAdres VARCHAR);
    FUNCTION PokazID (pNazwa VARCHAR) RETURN NATURAL;
    FUNCTION PokazNazwa (pID NATURAL) RETURN VARCHAR;
    FUNCTION PokazAdres (pID NATURAL) RETURN VARCHAR;
    exNazwaZespoluNieIstnieje EXCEPTION;
    exIDZespoluNieIstnieje EXCEPTION;
    exDuplikatIDZespolu EXCEPTION;
END IntZespoly;
/

CREATE OR REPLACE PACKAGE BODY IntZespoly IS

    PROCEDURE DodajZespol (pID NATURAL, pNazwa VARCHAR, pAdres VARCHAR) IS
    BEGIN
        INSERT INTO Zespoly VALUES (pID, pNazwa, pAdres);
        IF SQL%NOTFOUND THEN
            DBMS_OUTPUT.PUT_LINE ('Nie udało się dodać zespołu');
        END IF;
    EXCEPTION
        WHEN DUP_VAL_ON_INDEX THEN
            RAISE exDuplikatIDZespolu;
    END DodajZespol;

    PROCEDURE UsunZespolPoID (pID NATURAL) IS
    BEGIN
        DELETE FROM Zespoly WHERE id_zesp = pID;
        IF SQL%NOTFOUND THEN
            DBMS_OUTPUT.PUT_LINE ('Nie udało się usunąć zespołu');
        END IF;
    EXCEPTION
        WHEN NO_DATA_FOUND THEN
            RAISE exIDZespoluNieIstnieje;
    END UsunZespolPoID;

    PROCEDURE UsunZespolPoNazwie (pNazwa VARCHAR) IS
    BEGIN
        DELETE FROM Zespoly WHERE nazwa = pNazwa;
        IF SQL%NOTFOUND THEN
            DBMS_OUTPUT.PUT_LINE ('Nie udało się usunąć zespołu');
        END IF;
    EXCEPTION
        WHEN NO_DATA_FOUND THEN
            RAISE exNazwaZespoluNieIstnieje;
    END UsunZespolPoNazwie;

    PROCEDURE EdytujZespol (pID NATURAL, pNazwa VARCHAR, pAdres VARCHAR) IS
    BEGIN
        UPDATE Zespoly SET nazwa = pNazwa WHERE id_zesp = pID;
        UPDATE Zespoly SET adres = pAdres WHERE id_zesp = pID;
        IF SQL%NOTFOUND THEN
            DBMS_OUTPUT.PUT_LINE ('Nie udało się edytować zespołu');
        END IF;
    EXCEPTION
        WHEN DUP_VAL_ON_INDEX THEN
            RAISE exDuplikatIDZespolu;
    END EdytujZespol;

    FUNCTION PokazID (pNazwa VARCHAR) RETURN NATURAL IS
        vResult NATURAL;
    BEGIN
        SELECT id_zesp INTO vResult FROM zespoly WHERE nazwa = pNazwa;
        RETURN vResult;
    EXCEPTION
        WHEN NO_DATA_FOUND THEN
            RAISE exNazwaZespoluNieIstnieje;
    END PokazID;

    FUNCTION PokazNazwa (pID NATURAL) RETURN VARCHAR IS
        vResult VARCHAR(100);
    BEGIN
        SELECT nazwa INTO vResult FROM zespoly WHERE id_zesp = pID;
        RETURN vResult;
    EXCEPTION
        WHEN NO_DATA_FOUND THEN
            RAISE exIDZespoluNieIstnieje;
    END PokazNazwa;

    FUNCTION PokazAdres (pID NATURAL) RETURN VARCHAR IS
        vResult VARCHAR(100);
    BEGIN
        SELECT adres INTO vResult FROM zespoly WHERE id_zesp = pID;
        RETURN vResult;
    EXCEPTION
        WHEN NO_DATA_FOUND THEN
            RAISE exIDZespoluNieIstnieje;
    END PokazAdres;

END IntZespoly;
/