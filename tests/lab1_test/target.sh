#!/bin/bash
for file in ./*.frag; do
    ../../compiler467 -Tn "$file" > "$file".target 2>&1
done
