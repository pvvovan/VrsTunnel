#!/usr/bin/env bash

COVDATA=$1

SCRIPT_DIR=$(dirname "$(readlink -f $0)")
cd ${SCRIPT_DIR}

reportgenerator \
-reports:"${COVDATA}" \
-targetdir:"TestResults/coveragereport" \
-reporttypes: Html
