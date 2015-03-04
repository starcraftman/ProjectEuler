#!/usr/bin/env bash
# Run this to build project with cmake
# Alternative to my normal makefiles
ROOT=$(readlink -f $(dirname $0))
BDIR=$ROOT/gen

usage() {
  echo "Builds the project. Takes optional args:
  clean: Remove build dir.
  run  : Execute main program.
  test : Run tests."
}

build() {
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
    test)
      build
      echo "Solutions available in: $BDIR/src/"
      "$BDIR/src/Euler012_thread_test"
      ;;
    *) # Default
      command rm "$BDIR/src/Euler0$arg"
      build
      echo "Solutions available in: $BDIR/src/"
      "$BDIR/src/Euler0$arg"
      ;;
  esac
done
