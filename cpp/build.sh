#!/usr/bin/env bash
# Run this to build project with cmake
# Alternative to my normal makefiles
ROOT=$(readlink -f $(dirname $0))
BDIR=$ROOT/gen
BUILT=0

usage() {
  echo "Builds the project. Takes optional args:
  clean: Remove build dir.
  run  : Execute main program.
  test : Run tests."
}

build() {
  if [ $BUILT -eq 1 ]; then
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

for arg; do
  case "$arg" in
    *help|-h)
      usage
      exit
      ;;
    clean)
      rm -rf "$BDIR"
      mkdir -p "$BDIR"
      touch "$BDIR/DUMMY"
      ;;
    lib)
      build
      "$BDIR/util/LibTest"
      ;;
    travis)
      build
      # Ignore it for now
      GLOBIGNORE=$BDIR/src/Euler012*
      echo $GLOBIGNORE
      "$BDIR/util/LibTest"
      TESTS=( $BDIR/src/Euler* )
      for tcase in "${TESTS[@]}"; do
        "$tcase"
      done
      ;;
    test)
      build
      $BDIR/src/Euler*
      ;;
    *) # Default
      # Force recompilation for single problem runs
      command rm "$BDIR/src/Euler0$arg"
      build
      "$BDIR/src/Euler0$arg"
      ;;
  esac
done
