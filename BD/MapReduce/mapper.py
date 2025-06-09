#!/usr/bin/env python3

import sys
import re

p = re.compile(r'"\d+\/\d+\/(\d+)"')
INSTALL_DATE_COL = 4
INSTALL_DOCKCOUNT_COL = 5

for line in sys.stdin:
    values = line.split(",")
    
    install_date = values[INSTALL_DATE_COL]
    install_dockcount = values[INSTALL_DOCKCOUNT_COL]
    m = p.match(install_date)
        
    if m:
        install_year = m.group(1)
        print(f"{install_year}\t{install_dockcount}")
