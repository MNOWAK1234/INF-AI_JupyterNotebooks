#!/usr/bin/env python3
import sys
from collections import defaultdict

# Combiner: Agreguje wartości na poziomie Mappera
def combiner():
    league_data = defaultdict(lambda: [0, 0, 0])  # [total_wage, total_age, player_count]

    for line in sys.stdin:
        league_id, values = line.strip().split("\t")
        wage, age, count = map(float, values.split(","))
        
        league_data[league_id][0] += wage
        league_data[league_id][1] += age
        league_data[league_id][2] += int(count)

    # Emitujemy wstępnie zsumowane dane dla każdej ligi
    for league_id, (total_wage, total_age, player_count) in league_data.items():
        print(f"{league_id}\t{total_wage},{total_age},{player_count}")

if __name__ == "__main__":
    combiner()
