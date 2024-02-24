#!/bin/bash -e

# @@@ sample begin 0:0

pushd $1 > /dev/null

shift

readonly BUILDS=$(ls */build.sh)
readonly BUILD_FIRST=$(echo "$BUILDS" | head -1)

if [[ "$1" = "-h" ]]; then
    eval $BUILD_FIRST -h
    exit
fi

FIRST_OPT="-C" #-cならlibgtest.aもクリーン

for build in $BUILDS
do
    echo execute $build $* $FIRST_OPT
    eval $build $* $FIRST_OPT
    FIRST_OPT=    #libgtest.aのクリーンは一度
done
# @@@ sample end
