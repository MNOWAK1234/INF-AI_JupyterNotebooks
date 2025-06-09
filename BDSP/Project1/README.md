# Projekt 1 - Esper

## Wprowadzenie

W ramach projektu Esper każda osoba ma przyporządkowany zestaw danych i zadań.

## Instrukcje

1. Pobierz repozytorium odpowiadające przydzielonemu Ci zestawowi.

```sh
git clone https://github.com/BigDataStreamProcessing/project1-. . .
```

2. Zapoznaj się z charakterystyką danych oraz opisem atrybutów zdarzeń.

3. Otwórz, a następnie zapoznaj się ze strukturą projektu.

4. Rozwiąż kolejne zadania zgodnie z instrukcją w przydzielonym Ci zestawie.

5. Pamiętaj, że Twoim celem jest opracowanie rozwiązania poszczególnych zadań. Nie jest narzucony sposób tego rozwiązania. Sposób dobierasz samodzielnie.

6. Dla każdego z zadań należy dostarczyć (zarejestrować w stosownej aktywności kursu) komplet poleceń EPL stanowiących rozwiązanie, z **wyłączeniem** definicji schematu źródłowych zdarzeń (w rozwiązaniu nie umieszczamy poleceń definiujących schemat źródłowych zdarzeń).

7. Kilka reguł związanych z wzorcami, które należy uwzględnić w swoich rozwiązaniach:
   - W przypadku poszukiwania serii/sekwencji zdarzeń o określonych własnościach stosujemy podejście zachłanne (_greedy_)<br>
     _przykład_: dla sekwencji zdarzeń `ABBBBC` i wzorcu `B+` niepoprawnym rozwiązaniem będzie instancja wzorca `B` lub `BB`
   - W wynikach nie mogą pojawiać się instancje wzorców oparte **w całości** na podzbiorze zdarzeń z innych instancji<br>
     _przykład_: jeśli w wyniku wykrywania wzorca uzyskujemy instancję `ABCD`, to instancja `BCD` nie jest poprawna, nawet jeśli spełnia ona także reguły wzorca
   - W wynikach mają prawo pojawiać się instancje, w których część zdarzeń została zawarta w innych instancjach wzorca<br>
     _przykład_: jeśli w wyniku pojawiania się sekwencji zdarzeń `ABCDE` zostanie wykryta instancja wzorca `ABC`, to instancja `BCD` jest także poprawna o ile spełnia reguły wzorca
   - Powyższe reguły mają zastosowanie o ile treść zadania nie stanowi inaczej

W razie pytań kontaktuj się z prowadzącym.
