#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <example> <filename.txt>"
    exit 1
fi

word=$1
file=$2

if [ ! -f "$file" ]; then
    echo "Error: File '$file' not found!"
    exit 1
fi

grep -oiw "$word" "$file" | sort
