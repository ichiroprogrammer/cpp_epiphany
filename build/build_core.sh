#!/bin/bash

readonly BASE_DIR=$(cd $(dirname $0); pwd)
readonly BASENAME="$(basename $0)"

readonly MD_GEN=$(cd $BASE_DIR/../../md_gen/export/; pwd)

readonly EXEC_DIR=$1
shift

if [[ $(uname) =~ ^[Ll]inux.*$ ]]; then
    readonly IS_LINUX=true
    readonly IS_CYGWIN=false
elif [[ $(uname) =~ ^CYGWIN.*$ ]]; then
    readonly IS_LINUX=false
    readonly IS_CYGWIN=true
else
    readonly IS_LINUX=false
    readonly IS_CYGWIN=false
fi

# scriptオプション
#   CLANG_BUILD     make clangを実行するかどうか。デフォルトは実行
#   SCAN_BUILD      make scan-buildを実行するかどうか。デフォルトは実行
#   SAN_BUILD       make sanを実行するかどうか。デフォルトは実行

set -e

if $IS_CYGWIN; then
    CLANG_BUILD=false
    SCAN_BUILD=false
    SAN_BUILD=false
fi

function help(){
    echo "$BASENAME  [option]"
    echo "    -a    : equal -ce"
    echo "    -c    : clean before build"
    echo "    -C    : clean libgtest.a if with -c"
    echo "    -d    : dry run"
    echo "    -e    : find files which encoding is not utf-8"
    echo "    -g    : build by g++ only"
    echo "    -i    : execute IT"
    echo "    -j N  : make -j N for sample build [default:0]"
    echo "    -h    : show this message"

    exit $1
}

# @@@ sample begin 0:0

CLEAN=false
CLEAN_GTEST=false
IT=false
DRY_RUN=false
CHECK_ENCODING=false

while getopts ":aCcdeghij:" flag; do
    case $flag in 
    a) CLEAN=true; CHECK_ENCODING=true ;;
    c) CLEAN=true ;; 
    C) CLEAN_GTEST=true ;; 
    d) DRY_RUN=true ;; 
    e) CHECK_ENCODING=true ;; 
    g) CLANG_BUILD=false; SCAN_BUILD=false; SAN_BUILD=false ;;
    i) IT=true ;; 
    j) PARALLEL="$OPTARG" ;; 
    h) help 0 ;; 
    \?) help 1 ;; 
    esac
done

# @@@ ignore begin
readonly PARA="-j$([ -z "$PARALLEL" ] && : || echo $PARALLEL)"

shift $(expr ${OPTIND} - 1)
set -x

function cmd_launcher()
{
    $DRY_RUN || eval "$@"
}

function find_not_utf8() {
    pushd $EXEC_DIR > /dev/null

    cmd_launcher $MD_GEN/sh/find_not_utf8.sh

    popd > /dev/null
}

function clean() {
    pushd $EXEC_DIR > /dev/null

    cmd_launcher make clean
    $CLEAN_GTEST && cmd_launcher make clean_gtest

    popd > /dev/null
}

# @@@ ignore end

function build() {
    pushd $EXEC_DIR > /dev/null

    cmd_launcher make $PARA format

    $SCAN_BUILD && cmd_launcher make $PARA scan-build
    cmd_launcher make $PARA
    $CLANG_BUILD && cmd_launcher make $PARA clang
    $SAN_BUILD && cmd_launcher make $PARA san

    popd > /dev/null
}

function ut() {
    pushd $EXEC_DIR > /dev/null

    cmd_launcher make $PARA ut
    $CLANG_BUILD && cmd_launcher make $PARA clang-ut
    $SAN_BUILD && cmd_launcher make $PARA san-ut

    popd > /dev/null
}

function it() {
    pushd $EXEC_DIR > /dev/null

    cmd_launcher make $PARA it
    $CLANG_BUILD && cmd_launcher make $PARA O=clang++/ it
    $SAN_BUILD && cmd_launcher make $PARA O=sanitizer/ it

    popd > /dev/null
}

$CHECK_ENCODING && find_not_utf8

$CLEAN && clean

build

ut

$IT && it

exit 0
# @@@ sample end
