#!/bin/bash

gcc ReaderWriter.c -o ReaderWriter

if [ $? -eq 0 ]; then
  echo "Compilation successful!"
else
  echo "Compilation failed."
fi

