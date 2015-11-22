#!/bin/bash
echo ""
for file in ./*.frag; do
    ../../compiler467 -Da "$file" > "$file".out 2>&1
    DIFF=$(diff "$file".out "$file".target)
    if [ "$DIFF" != "" ] 
    then
        printf "diff   %-30s --- \e[1;31mFAILED\e[1;0m\n" "$file"
        echo "*****TEST $file FAILED TO MATCH TARGET*****"
        diff "$file".out "$file".target
    else
        printf "diff   %-30s --- \e[1;32mOK\e[1;0m\n" "$file"
    fi
done
rm -f *.out
