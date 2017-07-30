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
rm -rf debian/subutai-launcher
#rm -rf debian/poco

mkdir -p debian/python
#mkdir -p debian/poco

cp $launcher debian/SubutaiLauncher
#cp ../../bin/libsubutai-launcher.so debian/libsubutai-launcher.so
cp -r $pythondir/* debian/python/
#cp -r /usr/local/lib/libPoco*.so* debian/poco

# TODO: Add changelog converter
# TODO: Add build scheme

debuild -B -d
