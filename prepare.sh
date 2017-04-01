#!/bin/bash
# This script will download, compile and install latest Python into /usr/local/subutai directory
# This is a required step in order to make a distributable Launcher version

dir=`pwd`
top_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
python_dir="python_src"
dst_dir=/usr/local/share/subutai/python

if [ -d "$top_dir/$python_dir" ]; then
    echo "Python source exists. Updating"
    cd $top_dir/$python_dir
    git reset --hard
    git pull
else
    echo "Downloading Python source"
    git clone git@github.com:python/cpython.git $top_dir/$python_dir
    cd $top_dir/$python_dir
fi

./configure --prefix=$dst_dir --exec_prefix=$dst_dir
make
mkdir -p $dst_dir
if [ $? == 0 ]; then
    make install
else
    mkdir $dst_dir
    sudo make install
fi

cd $dir
