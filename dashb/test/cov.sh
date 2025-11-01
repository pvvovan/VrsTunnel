#!/usr/bin/env bash

COVDATA=$1

reportgenerator \
-reports:"${COVDATA}" \
-targetdir:"TestResults/coveragereport" \
-reporttypes: Html
