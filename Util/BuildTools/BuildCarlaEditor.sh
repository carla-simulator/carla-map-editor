#! /bin/bash

# ==============================================================================
# -- Set up environment --------------------------------------------------------
# ==============================================================================

source $(dirname "$0")/Environment.sh

if [ ! -d "${UE4_ROOT}" ]; then
  fatal_error "UE4_ROOT is not defined, or points to a non-existant directory, please set this environment variable."
else
  log "Using Unreal Engine at '$UE4_ROOT'"
fi

# ==============================================================================
# -- Parse arguments -----------------------------------------------------------
# ==============================================================================

DOC_STRING="Build and launch Carla Editor."

USAGE_STRING="Usage: $0 [-h|--help] [--build] [--rebuild] [--launch] [--clean] [--hard-clean]"

REMOVE_INTERMEDIATE=false
HARD_CLEAN=false
BUILD_CARLA_EDITOR=false
LAUNCH_UE4_EDITOR=false

OPTS=`getopt -o h --long help,build,rebuild,launch,clean,hard-clean -n 'parse-options' -- "$@"`
ED="/home/aclear/carla/Unreal/CarlaEditor"

if [ $? != 0 ] ; then echo "$USAGE_STRING" ; exit 2 ; fi

eval set -- "$OPTS"

while true; do
  case "$1" in
    --build )
      BUILD_CARLA_EDITOR=true;
      shift ;;
    --rebuild )
      REMOVE_INTERMEDIATE=true;
      BUILD_CARLA_EDITOR=true;
      shift ;;
    --launch )
      LAUNCH_UE4_EDITOR=true;
      shift ;;
    --clean )
      REMOVE_INTERMEDIATE=true;
      shift ;;
    --hard-clean )
      REMOVE_INTERMEDIATE=true;
      HARD_CLEAN=true;
      shift ;;
    -h | --help )
      echo "$DOC_STRING"
      echo "$USAGE_STRING"
      exit 1
      ;;
    * )
      break ;;
  esac
done

if ! { ${REMOVE_INTERMEDIATE} || ${BUILD_CARLA_EDITOR} || ${LAUNCH_UE4_EDITOR}; }; then
  fatal_error "Nothing selected to be done."
fi

pushd "${CARLA_EDITOR_ROOT_FOLDER}" >/dev/null

# ==============================================================================
# -- Clean Carla Editor ------------------------------------------------------------
# ==============================================================================

if ${HARD_CLEAN} ; then

  if [ ! -f Makefile ]; then
    fatal_error "The project wasn't built before!"
  fi

  log "Doing a \"hard\" clean of the Unreal Engine project."

  make CarlaEditorEditor ARGS=-clean

fi

if ${REMOVE_INTERMEDIATE} ; then

  log "Cleaning intermediate files and folders."

  UE4_INTERMEDIATE_FOLDERS="Binaries Build Intermediate DerivedDataCache"

  rm -Rf ${UE4_INTERMEDIATE_FOLDERS}

  rm -f Makefile

  pushd "${CARLA_EDITOR_PLUGIN_ROOT_FOLDER}" >/dev/null

  rm -Rf ${UE4_INTERMEDIATE_FOLDERS}

  popd >/dev/null

fi

# ==============================================================================
# -- Build CarlaEditor ------------------------------------------------------------
# ==============================================================================

if ${BUILD_CARLA_EDITOR} ; then

  if [ ! -f Makefile ]; then

    # This command fails sometimes but normally we can continue anyway.
    set +e
    log "Generate Unreal project files."
    ${UE4_ROOT}/GenerateProjectFiles.sh -project="${PWD}/CarlaEditor.uproject" -game -engine -makefiles
    set -e

  fi

  log "Build CarlaEditor project."
  make CarlaEditorEditor

fi

# ==============================================================================
# -- Launch UE4Editor ----------------------------------------------------------
# ==============================================================================

if ${LAUNCH_UE4_EDITOR} ; then

  log "Launching UE4Editor..."
  ${UE4_ROOT}/Engine/Binaries/Linux/UE4Editor "${PWD}/CarlaEditor.uproject"

else

  log "Success!"

fi

# ==============================================================================
# -- ...and we are done --------------------------------------------------------
# ==============================================================================

popd >/dev/null
