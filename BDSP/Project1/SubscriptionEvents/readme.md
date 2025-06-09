# Charakterystyka danych
Poszczególne osoby danej płci w wieku 18-100 wykupują subskrypcje do różnych serwisów streamingowych.

W strumieniu pojawiają się zdarzenia zgodne ze schematem `SubscriptionEvent`.

```
create json schema SubscriptionEvent(gender string, age int, service string, paymentMethod string, plan string, price int, ets string, its string);
```

Każde zdarzenie związane jest z wykupieniem subskrypcji przez daną osobę. 
W pojedynczej subskrypcji określona jest nazwa serwisu streamingowego, cena, plan oraz metoda płatności. 

Dane uzupełnione są o dwie etykiety czasowe. 
* Pierwsza (`ets`) związana jest z momentem wykupienia subskrypcji. 
  Etykieta ta może się losowo spóźniać w stosunku do czasu systemowego maksymalnie do 30 sekund.
* Druga (`its`) związana jest z momentem rejestracji zdarzenia wykupienia subskrypcji w systemie.

# Opis atrybutów

Atrybuty w każdym zdarzeniu zgodnym ze schematem `SubscriptionEvent` mają następujące znaczenie:

* `gender` - płeć (niebinarna — więcej niż 2 wartości) osoby, która wykupiła subskrypcję
* `age` - wiek osoby, która wykupiła subskrypcję (18-100)
* `service` - serwis streamingowy, na którym wykupiono subskrypcję
* `paymentMethod` - metoda płatności za subskrypcję
* `plan` - plan subskrypcji
* `price` - cena za subskrypcję (40-100)
* `ets` - czas wykupienia subskrypcji
* `its` - czas rejestracji faktu wykupienia subskrypcji w systemie

# Zadania
Opracuj rozwiązania poniższych zadań. 
* Opieraj się strumieniu zdarzeń zgodnych ze schematem `SubscriptionEvent`
* W każdym rozwiązaniu możesz skorzystać z jednego lub kilku poleceń EPL.
* Ostatnie polecenie będące ostatecznym rozwiązaniem zadania musi 
  * być poleceniem `select` 
  * posiadającym etykietę `answer`, przykładowo:
  
```aidl
@name('answer') SELECT gender, age, service, paymentMethod, plan, price, ets, its
FROM SubscriptionEvent#ext_timed(java.sql.Timestamp.valueOf(its).getTime(), 3 sec)
```

## Zadanie 1
Utrzymuj informacje dotyczące tego, jaka jest średnia cena subskrypcji wykupywanych
przez osoby poszczególnych płci w zdarzeniach zarejestrowanych w ciągu ostatnich 30 sekund.

Wyniki powinny zawierać, następujące kolumny:
- `gender` - płeć osoby, która wykupiła subskrypcję
- `avg_price` - średnia cena subskrypcji

## Zadanie 2
Wykrywaj przypadki wykupienia subskrypcji przez 100-letnie kobiety (gender = 'Female').

Wyniki powinny zawierać, następujące kolumny:
- `gender` - płeć osoby, która wykupiła subskrypcję
- `age` - wiek osoby, która wykupiła subskrypcję
- `service` - serwis streamingowy, na którym wykupiono subskrypcję
- `ets` - czas wykupienia subskrypcji
- `its` - czas rejestracji faktu wykupienia subskrypcji w systemie

## Zadanie 3
Znajduj przypadki, w których wiek osoby wykupującej subskrypcję w danym planie będzie o przynajmniej 10 lat wyższy niż średni wiek osób, które dokonały subskrypcji tego samego planu, zarejestrowanych w ciągu ostatnich 30 sekund.

Wyniki powinny zawierać, następujące kolumny:
- `plan` - plan subskrypcji
- `avg_age` - średni wiek osoby dla ostatnich 30 sekund
- `avg_price` - średnia cena subskrypcji dla ostatnich 30 sekund
- `age` - wiek osoby, która wykupiła subskrypcję
- `price` - cena za subskrypcję
- `ets` - czas wykupienia subskrypcji
- `its` - czas rejestracji faktu wykupienia subskrypcji w systemie

## Zadanie 4
Dla dwóch różnych grup wiekowych: (1) mających więcej niż 60 lat, (2) nieprzekraczających 60 lat, utrzymywane są sumaryczne wartości subskrypcji zarejestrowane w ciągu ostatniej minuty dla każdego z serwisów. 

Porównaj ze sobą serwisy dla obu grup wiekowych. 

Wyniki powinny zawierać następujące kolumny:

- `service` - nazwa serwisu
- `old_price_sum` - suma wartości subskrypcji do danego serwisu dla starszej grupy wiekowej 
- `young_price_sum` - suma wartości subskrypcji do danego serwisu dla młodszej grupy wiekowej 

## Zadanie 5
Dla każdej kolejnej minuty wyświetlaj pierwszego studenta, który wykupił subskrypcję w ciągu pierwszych 5 sekund trwania tej minuty.
Studenta identyfikujmy jako użytkownika do 26 roku życia (włącznie), który wykupił plan 'Student'.

Wyniki powinny zawierać, następujące kolumny:

- `age` - wiek osoby, która wykupiła subskrypcję
- `plan` - plan subskrypcji
- `service` - serwis streamingowy, na którym wykupiono subskrypcję
- `ets` - czas wykupienia subskrypcji
- `its` - czas rejestracji faktu wykupienia subskrypcji w systemie

## Zadanie 6
Wyświetlaj przypadki trzech występujących po sobie zakupów subskrypcji (niekoniecznie bezpośrednio po sobie) dokonanych przez osoby tej samej płci na kwotę powyżej 70. Zadbaj o to, aby pomiędzy tymi trzema zdarzeniami nie było żadnego, które dotyczy osoby innej płci.

Wyniki powinny zawierać następujące kolumny:

- `gender1` - płeć osoby z pierwszego zdarzenia
- `price1` - cena pierwszego zdarzenia
- `price2` - cena drugiego zdarzenia 
- `price3` - cena trzeciego zdarzenia

## Zadanie 7
Znajduj serie subskrypcji w tym samym serwisie, w których wiek osoby 
kupującej rośnie przez 3 kolejne zdarzenia. 

Wyniki powinny zawierać, następujące kolumny:

- `first_age` - wiek osoby z pierwszego zdarzenia
- `second_age` - wiek osoby z drugiego zdarzenia
- `third_age` - wiek osoby z trzeciego zdarzenia
- `service` - nazwa serwisu
