#!/usr/bin/env bash
# Run this to build project with cmake
# Alternative to my normal makefiles
ROOT="$(readlink -f $(dirname $0))"
BDIR="$ROOT/gen"
BUILT=0

usage() {
  echo "Builds the project. Takes optional args:
  clean  : Remove build dir.
  lib    : Build & run lib tests.
  travis : Run tests.
  *      : Problem number, build & run that problem. i.e. 22 -> run problem 22"
}

clean() {
  rm -rf "$BDIR"
  mkdir -p "$BDIR"
  touch "$BDIR/DUMMY"
}

travis_tests() {
  # Ignore it for now
  GLOBIGNORE=$BDIR/src/Euler012*

  TESTS=( "$BDIR/src/Euler"* )
  for tcase in "${TESTS[@]}"; do
    "$tcase"
  done
}

build() {
  if [ $BUILT -eq 1 -a $# -ne 1 ]; then
    return
  fi

  if [ "$(uname -s)" == "Linux" -a ! -d ./libs/lib ]; then
    echo "Warning, libs doesn't exists. Run GetLibsI.py"
    exit
  fi

  pushd "$BDIR"
  if [ ! -f "$BDIR/CMakeCache.txt" ]; then
    cmake ..
  fi
  make
  popd

  BUILT=1
  echo "Note: Solutions available in: $BDIR/src/"
}

shopt -s extglob # for +()

while (( $# > 0 )); do
  arg="$1"
  shift
  case "$arg" in
    *help|-h)
      usage
      exit
      ;;
    clean)
      clean
      ;;
    lib)
      build
      "$BDIR/util/LibTest"
      ;;
    travis)
      build
      travis_tests
      ;;
    +([0-9]))
      # Delete executable so run fails if compilation has errors
      command rm "$BDIR/src/Euler0$arg" 2>/dev/null
      build 1
      "$BDIR/src/Euler0$arg"
      ;;
    *) # Default
      echo "$arg: Not Recognized!"
      usage
      ;;
  esac
done
