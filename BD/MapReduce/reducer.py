#!/usr/bin/env python3

import sys

current_install_year = None
current_install_dockcount = 0
current_install_count = 0

for line in sys.stdin:
    install_year, install_dockcount = line.strip().split("\t")
    install_dockcount = int(install_dockcount.strip())
    if current_install_year == install_year:
        current_install_dockcount += install_dockcount
        current_install_count += 1
    else:
        if current_install_year:
            print(f"{current_install_year}\t{current_install_dockcount/current_install_count}")
        current_install_year = install_year
        current_install_count = 1
        current_install_dockcount = install_dockcount

if current_install_year == install_year:
    print(f"{current_install_year}\t{current_install_dockcount/current_install_count}")
