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
# clean
rm -rf flat
rm -rf root
rm -f *.pkg
version=`cat ../../VERSION`
# Copy files
mkdir -p flat/Resources/en.lproj
mkdir -p flat/base.pkg
#mkdir -p root/Applications/SubutaiLauncher.app/Contents/Frameworks
mkdir -p root/Applications/SubutaiLauncher.app/Contents/MacOS
#mkdir -p root/Applications/SubutaiLauncher.app/Contents/PlugIns
mkdir -p root/Applications/SubutaiLauncher.app/Contents/Resources

cp $launcher root/Applications/SubutaiLauncher.app/Contents/MacOS/SubutaiLauncher
mkdir -p root/usr/local/share/subutai/python; cp -r $pythondir/* root/usr/local/share/subutai/python/

cp PkgInfo.tmpl root/Applications/SubutaiLauncher.app/Contents/PkgInfo
cp Info.plist.tmpl root/Applications/SubutaiLauncher.app/Contents/Info.plist

cp ../../assets/dev/launcher-logo.icns root/Applications/SubutaiLauncher.app/Contents/Resources/SubutaiLauncher.icns
cp ../../assets/dev/*.ttf root/Applications/SubutaiLauncher.app/Contents/Resources/

# Determine sizes and modify PackageInfo
rootfiles=`find root | wc -l`
rootsize=`du -b -s root | awk '{print $1}'`
mbsize=$(( ${rootsize%% *} / 1024 ))

echo "Size: $rootsize"
echo "MBSize: $mbsize"

cp ./PackageInfo.tmpl ./flat/base.pkg/PackageInfo
sed -i -e "s/{VERSION_PLACEHOLDER}/$version/g" ./flat/base.pkg/PackageInfo
sed -i -e "s/{SIZE_PLACEHOLDER}/$mbsize/g" ./flat/base.pkg/PackageInfo
sed -i -e "s/{FILES_PLACEHOLDER}/$rootfiles/g" ./flat/base.pkg/PackageInfo

# modify Distribution
cp ./Distribution.tmpl ./flat/Distribution
sed -i -e "s/{VERSION_PLACEHOLDER}/$version/g" ./flat/Distribution
sed -i -e "s/{SIZE_PLACEHOLDER}/$mbsize/g" ./flat/Distribution

# Pack and bom
( cd root && find . | cpio -o --format odc --owner 0:80 | gzip -c ) > flat/base.pkg/Payload
( cd scripts && find . | cpio -o --format odc --owner 0:80 | gzip -c ) > flat/base.pkg/Scripts
mkbom -u 0 -g 80 root flat/base.pkg/Bom
( cd flat && xar --compression none -cf "../SubutaiLauncher-$version-Installer.pkg" * )
