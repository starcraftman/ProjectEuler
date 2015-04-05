#!/usr/bin/env bash
# Run this to build project with cmake
# Alternative to my normal makefiles
ROOT="$(readlink -f $(dirname $0))"
BDIR="$ROOT/gen"
BUILT=0

usage() {
  echo "Builds the project. Takes optional args:
  clean  : Remove build dir.
  prof   : Compile with profiling enabled (slower).
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
  # If want to ignore some tests, use below
  #GLOBIGNORE=$BDIR/src/Euler012*
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
    echo "Warning, libs doesn't exists. Run bootstrap.sh"
    exit
  fi

  pushd "$BDIR"
  if [ ! -f "$BDIR/CMakeCache.txt" ]; then
    if type -t 'ninja' > /dev/null 2>&1; then
      cmake -G "Ninja" $CMAKE_OPTS ..
    else
      cmake -G "Unix Makefiles" $CMAKE_OPTS ..
    fi
  fi

  if type -t 'ninja' > /dev/null 2>&1; then
    ninja
  else
    make -j
  fi
  popd

  BUILT=1
  echo "Note: Solutions available in: $BDIR/src/"
}

shopt -s extglob # for +()

if [ "$#" -eq 0 ]; then
  usage
  exit
fi

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
    prof*)
      CMAKE_OPTS+="-D GPROF=ON "
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
