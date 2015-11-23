#!/bin/bash
for folder in ./lab*_test; do
    echo "============== TEST BEGIN FOR " "$folder" "==============" 
    cd $folder
    sh ./run.sh
    cd ..
done
