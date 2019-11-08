#!/usr/bin/env bash

#
# Build the Debian artifacts
#
set -xe
sudo apt-get -qq update
sudo apt-get install devscripts equivs

rm -rf build && mkdir build && cd build
mk-build-deps ../ci/control
sudo apt-get install  ./*all.deb  || :
sudo apt-get --allow-unauthenticated install -f
rm -f ./*all.deb

cmake -DCMAKE_BUILD_TYPE=Release ..
make -sj2
make package
