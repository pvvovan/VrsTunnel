#!/usr/bin/env bash

SCRIPT_DIR=$(dirname "$(readlink -f $0)")
cd ${SCRIPT_DIR}

find ${SCRIPT_DIR}/TestResults/ -name "*.cobertura.xml" | xargs -n1 rm

dotnet test --logger html --collect:"XPlat Code Coverage"

COVDATA=$(find ${SCRIPT_DIR}/TestResults/ -name "*.cobertura.xml")

reportgenerator \
-reports:"${COVDATA}" \
-targetdir:"TestResults/coveragereport" \
-reporttypes: Html
