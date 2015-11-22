#!/bin/bash
for file in ./*.frag; do
    ../../compiler467 -Da "$file" > "$file".target 2>&1
done
