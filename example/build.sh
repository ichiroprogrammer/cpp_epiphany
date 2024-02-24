#!/bin/bash -e

readonly BASE_DIR=$(cd $(dirname $0); pwd)
readonly BASENAME="$(basename $0)"

$BASE_DIR/../build/build_foreach.sh $BASE_DIR $@
