#!/usr/bin/env python3
import sys
from collections import defaultdict

# Reducer: Finalnie sumuje i oblicza średnie wartości dla każdej ligi
def reducer():
    league_data = defaultdict(lambda: [0, 0, 0])  # [total_wage, total_age, player_count]

    for line in sys.stdin:
        league_id, values = line.strip().split("\t")
        total_wage, total_age, count = map(float, values.split(","))

        # Sumowanie danych z Mappera/Combinerów
        league_data[league_id][0] += total_wage
        league_data[league_id][1] += total_age
        league_data[league_id][2] += int(count)

    # Obliczamy średnie i wypisujemy wynik
    for league_id, (total_wage, total_age, player_count) in league_data.items():
        if player_count > 0:
            avg_wage = total_wage / player_count
            avg_age = total_age / player_count
            print(f"{league_id};{avg_wage:.2f};{avg_age:.2f};{player_count}")

if __name__ == "__main__":
    reducer()
