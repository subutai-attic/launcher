#!/bin/bash

launcher=$1
pythondir=$2

if [ ! -f "$launcher" ]; then
    echo "Couldn't find specified file: $launcher"
    exit 1
fi
if [ ! -d "$pythondir" ]; then
    echo "Couldn't find $python directory"
    exit 1
fi

rm -rf debian/python
rm -rf debian/SubutaiLauncher

mkdir -p debian/python

cp $launcher debian/SubutaiLauncher
cp -r $pythondir/* debian/python/

# TODO: Add changelog converter
# TODO: Add build scheme

debuild -B -d
