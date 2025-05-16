# Term 5: SE (Software Engineering)

For building administrators who want to optimize building management costs, our Building Info application will enable the acquisition of information about building parameters at the level of rooms, floors and entire buildings. The application will be available through a GUI and also as a remote API so it can be integrated with existing tools.

# General Info

- A Location is a Building, a Floor, or a Room
- A Building has a list of Floors, each of which have a list of rooms on corresponding floor
- Each location has:
  - number (int) - a unique Number (Roomnumber, Floornumber, Buildingnumber)
- a room has also these parameters:
  - SurfaceArea = rooms surface in m^2
  - Volume - rooms volume
  - LightIntensity – How bright the room is
  - EnergyConsumption - How energy consuming the room is

# Contributors

- **PPO** – ['KotZPolibudy'](https://github.com/KotZPolibudy)
- **Scrum Master** – ['GaussX0'](https://github.com/GaussX0)
- **Developer** – ['kilianczyko'](https://github.com/kilianczyko)
- **Developer** – ['MNOWAK1234'](https://github.com/MNOWAK1234)

# Continuous Integration server status

![ci status](https://github.com/MNOWAK1234/PUTcourses/actions/workflows/ci.yml/badge.svg)

# PL - IOD-L11-Gamma

Dla administratorów budynków, którzy pragną optymalizować koszty zarządzania budynkami nasza aplikacja Building Info umożliwi pozyskanie informacji o parametrach budynku na poziomie pomieszczeń, kondygnacji oraz całych budynków. Aplikacja będzie dostępna poprzez GUI a także jako zdalne API dzięki czemu można ją zintegrować z istniejącymi narzędziami.

# Ogólne informacje

- Lokacja to budynek, poziom, lub pomieszczenie
- Budynek może składać się z poziomów a te z pomieszczeń
- Każda lokalizacja jest charakteryzowana przez:
  - numer – numer pokoju, unikalny identyfikator
- Pomieszczenie dodatkowo jest charakteryzowane przez:
  - Surface = powierzchnia w m^2
  - Volume = kubatura pomieszczenia w m^3
  - LightIntensity – natężenie oświetlenia w danym miejscu
  - EnergyConsumption = poziom zużycia energii ogrzewania

# Współautorzy

- **PPO** – ['KotZPolibudy'](https://github.com/KotZPolibudy)
- **Scrum Master** – ['GaussX0'](https://github.com/GaussX0)
- **Developer** – ['kilianczyko'](https://github.com/kilianczyko)
- **Developer** – ['MNOWAK1234'](https://github.com/MNOWAK1234)

# Status serwera ciągłej integracji

![ci status](https://github.com/MNOWAK1234/PUTcourses/actions/workflows/ci.yml/badge.svg)
