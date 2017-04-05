#!/bin/bash

launcher=$1
pythondir=$2
if [ ! -f $launcher ]; then
    echo "Couldn't find specified file: $launcher"
    exit 1
fi
if [ ! -d $pythondir ]; then
    echo "Couldn't find $python directory"
    exit 1
fi
version=4.0.15
# Copy files
mkdir -p root/Applications; cp $launcher root/Applications/SubutaiLauncher.app
mkdir -p root/usr/local/share/subutai; cp -r $pythondir/* root/usr/local/share/subutai/python/
# Determine sizes and modify PackageInfo
rootfiles=`find root | wc -l`
rootsize=`du -b -s root | awk '{print $1}'`
cp ./PackageInfo.tmpl ./flat/base.pkg/PackageInfo
sed -i -e "s/{VERSION_PLACEHOLDER}/$version/g" ./flat/base.pkg/PackageInfo
sed -i -e "s/{SIZE_PLACEHOLDER}/$rootsize/g" ./flat/base.pkg/PackageInfo
sed -i -e "s/{FILES_PLACEHOLDER}/$rootfiles/g" ./flat/base.pkg/PackageInfo

# modify Distribution
cp ./Distribution.tmpl ./flat/Distribution
sed -i -e "s/{VERSION_PLACEHOLDER}/$version/g" ./flat/Distribution
sed -i -e "s/{SIZE_PLACEHOLDER}/$rootsize/g" ./flat/Distribution

# Pack and bom
( cd root && find . | cpio -o --format odc --owner 0:80 | gzip -c ) > flat/base.pkg/Payload
( cd scripts && find . | cpio -o --format odc --owner 0:80 | gzip -c ) > flat/base.pkg/Scripts
mkbom -u 0 -g 80 root flat/base.pkg/Bom
( cd flat && xar --compression none -cf "../SubutaiLauncher-4.0.15-Installer.pkg" * )
