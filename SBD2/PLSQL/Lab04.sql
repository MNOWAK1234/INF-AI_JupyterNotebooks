--- 1.
CREATE TABLE DziennikOperacji
(   data DATE,
    typ VARCHAR(10),
    tabela VARCHAR(20),
    liczba_rekordow INTEGER);
/
	
CREATE OR REPLACE TRIGGER LogujOperacje
    AFTER INSERT OR DELETE OR UPDATE ON Zespoly
BEGIN
    CASE
        WHEN INSERTING THEN
            INSERT INTO DziennikOperacji VALUES (CURRENT_DATE, 'INSERT', 'Zespoly', (SELECT COUNT(*) FROM Zespoly));
        WHEN DELETING THEN
            INSERT INTO DziennikOperacji VALUES (CURRENT_DATE, 'DELETE', 'Zespoly', (SELECT COUNT(*) FROM Zespoly));
        WHEN UPDATING THEN
            INSERT INTO DziennikOperacji VALUES (CURRENT_DATE, 'UPDATE', 'Zespoly', (SELECT COUNT(*) FROM Zespoly));
    END CASE;
END;
/

--- 2.
CREATE OR REPLACE TRIGGER PokazPlace
    BEFORE UPDATE OF placa_pod ON Pracownicy
    FOR EACH ROW
    WHEN (COALESCE(OLD.placa_pod,0) <> COALESCE(NEW.placa_pod,0))
BEGIN
    DBMS_OUTPUT.PUT_LINE('Pracownik ' || :OLD.nazwisko);
    DBMS_OUTPUT.PUT_LINE('Płaca przed modyfikacją: ' || :OLD.placa_pod);
    DBMS_OUTPUT.PUT_LINE('Płaca po modyfikacji: ' || :NEW.placa_pod);
END;
/

--- 3.
CREATE OR REPLACE TRIGGER UzupelnijPlace
    BEFORE INSERT ON Pracownicy
    FOR EACH ROW
    WHEN (NEW.placa_pod IS NULL OR NEW.placa_dod IS NULL)
DECLARE
    vPlacaMin Etaty.placa_min%TYPE;
BEGIN
    IF :NEW.placa_pod IS NULL AND :NEW.etat IS NOT NULL THEN
        SELECT placa_min
        INTO vPlacaMin
        FROM Etaty WHERE nazwa = :NEW.etat;
        :NEW.placa_pod := vPlacaMin;
    END IF;
    IF :NEW.placa_dod IS NULL THEN
        :NEW.placa_dod := 0;
    END IF;
END;
/

--- 4.
SELECT MAX(id_zesp) FROM Zespoly;
/
CREATE SEQUENCE SEQ_Zespoly START WITH 51 INCREMENT BY 1; 
/
CREATE OR REPLACE TRIGGER UzupelnijID
    BEFORE INSERT ON Zespoly
    FOR EACH ROW
    WHEN (NEW.id_zesp IS NULL)
BEGIN
    :NEW.id_zesp := SEQ_ZESPOLY.NEXTVAL;
END;
/

--- 5.
CREATE OR REPLACE VIEW Szefowie AS
SELECT p1.NAZWISKO AS SZEF, COUNT(p2.ID_PRAC) AS PRACOWNICY
FROM pracownicy p1
LEFT JOIN pracownicy p2 ON p1.ID_PRAC = p2.ID_SZEFA
GROUP BY p1.NAZWISKO
HAVING COUNT(p2.ID_PRAC) > 0;
/
SELECT * FROM SZEFOWIE
/
CREATE OR REPLACE TRIGGER UsunSzefa
INSTEAD OF DELETE ON Szefowie
FOR EACH ROW
DECLARE
   v_podwladni NUMBER;
   v_szef_exist NUMBER;
BEGIN
   FOR sub IN (
       SELECT NAZWISKO, ID_PRAC
       FROM pracownicy
       WHERE ID_SZEFA = (SELECT ID_PRAC FROM pracownicy WHERE NAZWISKO = :OLD.SZEF)
   )
   LOOP
      FOR sub_subordinate IN (
         SELECT NAZWISKO
         FROM pracownicy
         WHERE ID_SZEFA = sub.ID_PRAC
      )
      LOOP
         RAISE_APPLICATION_ERROR(-20001, 'Jeden z podwładnych usuwanego szefa jest szefem innych pracowników. Usuwanie anulowane!');
      END LOOP;
   END LOOP;

   DELETE FROM pracownicy
   WHERE ID_SZEFA = (SELECT ID_PRAC FROM pracownicy WHERE NAZWISKO = :OLD.SZEF);

   DELETE FROM pracownicy
   WHERE ID_PRAC = (SELECT ID_PRAC FROM pracownicy WHERE NAZWISKO = :OLD.SZEF);
END;
/
DELETE FROM szefowie WHERE szef='MORZY';
/
SELECT * FROM pracownicy
    WHERE id_prac = 140 OR id_szefa = 140;
/
SELECT * FROM PRACOWNICY;
/
ROLLBACK;
/
SELECT * FROM PRACOWNICY;

--- 6.
ALTER TABLE Zespoly
ADD liczba_pracownikow NUMBER(6) DEFAULT 0;
/
UPDATE Zespoly z
SET z.liczba_pracownikow = (
    SELECT COUNT(*) 
    FROM Pracownicy p
    WHERE p.ID_ZESP = z.ID_ZESP
);
/
ALTER TABLE Zespoly
ADD liczba_pracownikow NUMBER(6) DEFAULT 0;
/
UPDATE Zespoly z
SET z.liczba_pracownikow = (
    SELECT COUNT(*) 
    FROM Pracownicy p
    WHERE p.ID_ZESP = z.ID_ZESP
);
/
SELECT * FROM ZESPOLY;
/
CREATE OR REPLACE TRIGGER DodajPracownika
AFTER INSERT ON Pracownicy
FOR EACH ROW
BEGIN
    UPDATE Zespoly
    SET liczba_pracownikow = liczba_pracownikow + 1
    WHERE ID_ZESP = :NEW.ID_ZESP;
END;
/

CREATE OR REPLACE TRIGGER UsunPracownika
AFTER DELETE ON Pracownicy
FOR EACH ROW
BEGIN
    UPDATE Zespoly
    SET liczba_pracownikow = liczba_pracownikow - 1
    WHERE ID_ZESP = :OLD.ID_ZESP;
END;
/

CREATE OR REPLACE TRIGGER PrzesunPracownika
AFTER UPDATE OF ID_ZESP ON Pracownicy
FOR EACH ROW
BEGIN
    IF :OLD.ID_ZESP <> :NEW.ID_ZESP THEN
        UPDATE Zespoly
        SET liczba_pracownikow = liczba_pracownikow - 1
        WHERE ID_ZESP = :OLD.ID_ZESP;
        
        UPDATE Zespoly
        SET liczba_pracownikow = liczba_pracownikow + 1
        WHERE ID_ZESP = :NEW.ID_ZESP;
    END IF;
END;
/

--- 7.
ALTER TABLE Pracownicy
DROP CONSTRAINT FK_ID_SZEFA;
/
ALTER TABLE Pracownicy
ADD CONSTRAINT FK_ID_SZEFA
FOREIGN KEY (ID_SZEFA)
REFERENCES Pracownicy(ID_PRAC)
ON DELETE CASCADE;
/
CREATE OR REPLACE TRIGGER Usun_Prac_After
AFTER DELETE ON Pracownicy
FOR EACH ROW
BEGIN
   DBMS_OUTPUT.PUT_LINE('Usunięty pracownik: ' || :OLD.NAZWISKO);
END;
/
CREATE OR REPLACE TRIGGER Usun_Prac_Before
BEFORE DELETE ON Pracownicy
FOR EACH ROW
BEGIN
   DBMS_OUTPUT.PUT_LINE('Przed usunięciem pracownika: ' || :OLD.NAZWISKO);
END;
/
DELETE FROM Pracownicy WHERE NAZWISKO = 'MORZY';
/
ROLLBACK;

--- 8.
ALTER TABLE Pracownicy DISABLE ALL TRIGGERS;
/
SELECT TRIGGER_NAME, STATUS
FROM USER_TRIGGERS
WHERE TABLE_NAME = 'PRACOWNICY';
/

--- 9.
DROP TRIGGER DODAJPRACOWNIKA;
DROP TRIGGER PRZESUNPRACOWNIKA;
DROP TRIGGER USUNPRACOWNIKA;
DROP TRIGGER USUN_PRAC_AFTER;
DROP TRIGGER USUN_PRAC_BEFORE;
/
