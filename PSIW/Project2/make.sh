#!/bin/bash

gcc -pthread Walk.c -o walk

if [ $? -eq 0 ]; then
  echo "Compilation successful!"
else
  echo "Compilation failed."
fi

