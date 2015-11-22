#!/bin/bash
for file in ./*.frag; do
    ../../compiler467 -Tp "$file" > "$file".target 2>&1
done
