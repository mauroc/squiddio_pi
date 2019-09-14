#!/usr/bin/env bash

#
# Build the Travis Debian artifacts
#
set -xe
sudo apt-get -qq update
sudo apt-get install devscripts equivs

mkdir  build
cd build
<<<<<<< HEAD
=======
ln -s ../ci/control.${OCPN_TARGET} ../ci/control
>>>>>>> Update to new Travis process
mk-build-deps ../ci/control
sudo apt-get install  ./*all.deb  || :
sudo apt-get --allow-unauthenticated install -f

<<<<<<< HEAD
cmake -DCMAKE_BUILD_TYPE=Debug ..
=======
cmake -DCMAKE_BUILD_TYPE=Release ..
>>>>>>> Update to new Travis process
make -sj2
make package
