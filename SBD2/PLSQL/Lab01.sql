-- 1.
DECLARE
    vTekst varchar(20) DEFAULT 'Witaj, świecie!';
    vLiczba number(10,4) DEFAULT 1000.456;
BEGIN
    vTekst := 'Witaj świecie';
    vLiczba := 1000.456;
    DBMS_OUTPUT.PUT_LINE('Zmienna vTekst: ' || vTekst);
    DBMS_OUTPUT.PUT_LINE('Zmienna vLiczba: ' || vLiczba);
END;

-- 2.
DECLARE
    vTekst VARCHAR(50) := 'Witaj, świecie!';
    vLiczba NUMBER (20,3) := 1000.456;
BEGIN
    vTekst := vTekst || ' Witaj, nowy dniu!';
    vLiczba := vLiczba + 1000000000000000;
    DBMS_OUTPUT.PUT_LINE('Zmienna vTekst: ' || vTekst);
    DBMS_OUTPUT.PUT_LINE('Zmienna vLiczba: ' || vLiczba);
END;

-- 3.
DECLARE
    Jeden NUMBER (20, 7) := 10.2356000;
    Dwa NUMBER (20,7) := 0.0000001;
    Suma NUMBER (21,7) :=Jeden + Dwa;
BEGIN
    DBMS_OUTPUT.PUT_LINE('Wynik dodawania ' || Jeden || ' i ' || Dwa || ' to: ' || Suma);
END;

-- 4.
DECLARE
    Promien NUMBER (20,7) := 5;
    Obwod NUMBER (20,7);
    Pole NUMBER (21,7);
    cPI CONSTANT NUMBER(3,2) := 3.14; 
BEGIN
    Obwod := 2 * Promien * cPi;
    Pole := cPi * Promien * Promien;
    DBMS_OUTPUT.PUT_LINE('Obwód koła o promieniu równym ' || Promien || ': ' || Obwod);
    DBMS_OUTPUT.PUT_LINE('Pole koła o promieniu równym ' || Promien || ': ' || Pole);
END;

-- 5.
DECLARE
    vNazwisko Pracownicy.nazwisko%TYPE;
    vEtat Pracownicy.etat%TYPE;
    vPlaca Pracownicy.placa_pod%TYPE;
BEGIN
    SELECT nazwisko, etat, placa_pod
    INTO vNazwisko, vEtat, vPlaca
    FROM pracownicy 
    WHERE placa_pod = (SELECT max(placa_pod) FROM pracownicy);
    DBMS_OUTPUT.PUT_LINE('Najlepiej zarabia pracownik ' || vNazwisko);
    DBMS_OUTPUT.PUT_LINE('Pracuje on jako ' || vEtat);
END;

-- 6.
DECLARE
    vPracownik Pracownicy%ROWTYPE;
BEGIN
    SELECT *
    INTO vPracownik
    FROM pracownicy 
    WHERE placa_pod = (SELECT max(placa_pod) FROM pracownicy);
    DBMS_OUTPUT.PUT_LINE('Najlepiej zarabia pracownik ' || vPracownik.nazwisko);
    DBMS_OUTPUT.PUT_LINE('Pracuje on jako ' || vPracownik.etat);
END;

-- 7.
DECLARE
    SUBTYPE tPieniadze IS NUMBER(15,2);
    vZarobki tPieniadze;
BEGIN
    SELECT 12*(placa_pod+COALESCE(placa_dod,0))
    INTO vZarobki
    FROM pracownicy 
    WHERE nazwisko = 'SLOWINSKI';
    DBMS_OUTPUT.PUT_LINE('Pracownik SLOWINSKI zarabia rocznie ' || vZarobki);
END;

-- 8.
BEGIN    
    LOOP
        EXIT WHEN EXTRACT(SECOND FROM CURRENT_TIMESTAMP) = 25;
    END LOOP;
    DBMS_OUTPUT.PUT_LINE('Nadeszła 25 sekunda!');
END;

-- 9.
DECLARE
    vN NATURAL := 10;
    Silnia NATURAL := 1;
    Licznik NATURAL := 1;
BEGIN
    LOOP
        IF Licznik = vN THEN EXIT;
        END IF;
        Licznik := Licznik + 1;
        Silnia := Silnia * Licznik;
    END LOOP;
    DBMS_OUTPUT.PUT_LINE('Silnia dla n=' || vN || ': ' || Silnia);
END;

-- 10.
DECLARE
    vData DATE := DATE '2001-01-13';
BEGIN    
    LOOP
        EXIT WHEN EXTRACT(YEAR FROM vData) = 2101;
        IF TO_CHAR(vData,'D') = 5 THEN DBMS_OUTPUT.PUT_LINE(vData);
        END IF;
        vData := ADD_MONTHS(vData, 1);
    END LOOP;
END;
