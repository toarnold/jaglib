#!/bin/bash
mkdir -p obj
mkdir -p lib
docker run --rm -u=$UID:$(id -g $USER) --volume "$PWD":/usr/src/compile --workdir /usr/src/compile toarnold/jaguarvbcc:0.9fp1b make
