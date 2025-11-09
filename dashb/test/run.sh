#!/usr/bin/env bash

SCRIPT_DIR=$(dirname "$(readlink -f $0)")
cd ${SCRIPT_DIR}

dotnet test --logger html --collect:"XPlat Code Coverage"
