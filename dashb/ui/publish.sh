#!/usr/bin/env bash

# Publish ui app in a single file

SCRIPT_DIR=$(dirname "$(readlink -f $0)")
cd $SCRIPT_DIR

dotnet publish
