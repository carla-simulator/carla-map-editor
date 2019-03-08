#! /bin/bash

source $(dirname "$0")/Environment.sh

# ==============================================================================
# -- Parse arguments -----------------------------------------------------------
# ==============================================================================

DOC_STRING="Makes a packaged version of the CARLA Editor for distribution."

USAGE_STRING="Usage: $0 [-h|--help] [--no-packaging] [--no-zip] [--clean-intermediate]"

DO_PACKAGE=true
DO_COPY_FILES=true
DO_TARBALL=true
DO_CLEAN_INTERMEDIATE=false

OPTS=`getopt -o h --long help,no-packaging,no-zip,clean-intermediate -n 'parse-options' -- "$@"`

if [ $? != 0 ] ; then echo "$USAGE_STRING" ; exit 2 ; fi

eval set -- "$OPTS"

while true; do
  case "$1" in
    --no-packaging )
      DO_PACKAGE=false
      shift ;;
    --no-zip )
      DO_TARBALL=false
      shift ;;
    --clean-intermediate )
      DO_CLEAN_INTERMEDIATE=true
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

# ==============================================================================
# -- Package project -----------------------------------------------------------
# ==============================================================================

REPOSITORY_TAG=$(get_carla_version)

BUILD_FOLDER=${CARLA_DIST_FOLDER}/${REPOSITORY_TAG}

log "Packaging version '$REPOSITORY_TAG'."

if $DO_PACKAGE ; then

  pushd "${CARLA_EDITOR_ROOT_FOLDER}" >/dev/null

  log "Packaging the project."

  if [ ! -d "${UE4_ROOT}" ]; then
    fatal_error "UE4_ROOT is not defined, or points to a non-existant directory, please set this environment variable."
  fi

  rm -Rf ${BUILD_FOLDER}
  mkdir -p ${BUILD_FOLDER}

  ${UE4_ROOT}/Engine/Build/BatchFiles/RunUAT.sh BuildCookRun \
      -project="${PWD}/CarlaEditor.uproject" \
      -nocompileeditor -nop4 -cook -stage -archive -package \
      -clientconfig=Development -ue4exe=UE4Editor \
      -prereqs -nodebuginfo \
      -targetplatform=Linux -build -utf8output \
      -archivedirectory="${BUILD_FOLDER}"

  popd >/dev/null

  if [[ ! -d ${BUILD_FOLDER}/LinuxNoEditor ]] ; then
    fatal_error "Failed to package the project!"
  else
    pushd "${BUILD_FOLDER}/LinuxNoEditor/CarlaEditor" >/dev/null
    mkdir Import Export
    popd >/dev/null
  fi

fi



# ==============================================================================
# -- Zip the project -----------------------------------------------------------
# ==============================================================================

if $DO_TARBALL ; then

  DESTINATION=${CARLA_DIST_FOLDER}/CARLA_${REPOSITORY_TAG}.tar.gz
  SOURCE=${BUILD_FOLDER}/LinuxNoEditor

  pushd "${SOURCE}" >/dev/null

  log "Packaging build."

  rm -f ./Manifest_NonUFSFiles_Linux.txt
  rm -Rf ./CarlaEditor/Saved
  rm -Rf ./Engine/Saved

  tar -czvf ${DESTINATION} *

  popd >/dev/null

fi

# ==============================================================================
# -- Remove intermediate files -------------------------------------------------
# ==============================================================================

if $DO_CLEAN_INTERMEDIATE ; then

  log "Removing intermediate build."

  rm -Rf ${BUILD_FOLDER}

fi

# ==============================================================================
# -- ...and we are done --------------------------------------------------------
# ==============================================================================

if $DO_TARBALL ; then
  FINAL_PACKAGE=${CARLA_DIST_FOLDER}/CARLA_${REPOSITORY_TAG}.tar.gz
else
  FINAL_PACKAGE=${BUILD_FOLDER}
fi

log "Packaged version created at ${FINAL_PACKAGE}"
log "Success!"
