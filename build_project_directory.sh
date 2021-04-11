#!/bin/bash

create_readme_files () {
    filename=README.md
    cd $1
    if [[ ! -f "$filename" ]]; then
        echo "generating readme files $1"
        touch $filename;
    fi
    
    for dir in $1/*/; do
        if [[ -d "$dir" ]]; then
            create_readme_files $dir
        fi
    done

    cd -
}

create_readme_files .
