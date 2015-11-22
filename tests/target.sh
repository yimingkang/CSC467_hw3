#!/bin/bash
for folder in ./lab*_test; do
    echo "+++++++++++++++++++ CREATING TARGET FOR" "$folder" " ++++++++++++++++++++"
    cd $folder
    sh target.sh
    cd ..
done
