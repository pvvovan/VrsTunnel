#!/usr/bin/env bash

SCRIPT_DIR=$(dirname "$(readlink -f $0)")
cd ${SCRIPT_DIR}

RESULTS_DIR=${SCRIPT_DIR}/TestResults

if [ -d ${RESULTS_DIR} ]; then
    rm -r ${RESULTS_DIR}
fi

dotnet build ../vm/vm.csproj
dotnet build ../ui/ui.csproj
dotnet build ./test.csproj

roslynator analyze ../vm/vm.csproj ../ui/ui.csproj ./test.csproj --severity-level warning

dotnet test --logger html --collect:"XPlat Code Coverage"

COVDATA=$(find ${RESULTS_DIR} -name "*.cobertura.xml")

reportgenerator \
-reports:"${COVDATA}" \
-targetdir:"${RESULTS_DIR}/coveragereport" \
-reporttypes: Html
