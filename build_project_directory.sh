#!/bin/bash

PROJECT_NAME="ObstacleDetector"
PROJECTS_TOP="projects"
PROJECT_ROOT="$GIT_TOP/$PROJECTS_TOP/$PROJECT_NAME"

print_project_directories () {
    echo "Project root: $PROJECT_ROOT"
}

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

create_project_directories () {
    dirs="core utils test cmd "

    for d in $dirs; do
        if [[ ! -d "$d" ]]; then
            echo "Creating directory: $d"
            mkdir -p $d
        fi
    done
}

setup_git_repository () {
    GITHUB="https://github.com/CaesarJeries/"
    REPO_URL = "$GITHUB/$PROJECT_NAME.git"
    git remote add origin $REPO_URL
}


# todo: uncoment before release
# create_readme_files .


# do last
tree .

# todo: parse flags: --dry-run, --set-origin
