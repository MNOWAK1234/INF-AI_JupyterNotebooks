#!/usr/bin/env python3
import sys

# Mapper: Wczytuje dane z datasource1, filtruje i emituje wartości
def mapper():
    for line in sys.stdin:
        fields = line.strip().split(';')
        if len(fields) < 108:
            continue  # Pomiń linie niekompletne

        try:
            league_id = fields[16]
            wage_eur = float(fields[11])
            age = int(fields[12])
            weight_kg = int(fields[15])
            
            # Filtrujemy piłkarzy ważących ponad 100 kg
            if weight_kg < 100:
                # Emitujemy league_id jako klucz, a zarobki, wiek i licznik jako wartość
                print(f"{league_id}\t{wage_eur},{age},1")
        except ValueError:
            continue  # Pomiń linie z błędnymi wartościami

if __name__ == "__main__":
    mapper()
