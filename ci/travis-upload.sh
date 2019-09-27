#!/bin/sh
PLUGIN="squiddio"

VERSION="unstable"
OPTS="override=1;publish=1"
PKG="plugins"

API_BASE="https://api.bintray.com/content/leamas/OpenCPN/$PKG/$VERSION"

cd build
tarball=$(echo ${PLUGIN}*.tar.gz)
xml=$(echo ${PLUGIN}-plugin*.xml)

BINTRAY_API_KEY="50a571aa02e1a7dd0ff1439efd0b3b3f4681b74f"

if [ -z "$BINTRAY_API_KEY" ]; then
    echo 'Cannot deploy to bintray:: missing $BINTRAY_API_KEY'
else
    echo "Deploying to bintray"
    set -x
    curl -T $tarball --user mauroc:$BINTRAY_API_KEY "$API_BASE/$tarball;$OPTS"
    curl -T $xml --user mauroc:$BINTRAY_API_KEY "$API_BASE/$xml;$OPTS"
    set +x
fi


if [ -z "$CLOUDSMITH_API_KEY" ]; then
    echo 'Cannot deploy to cloudsmith: missing $CLOUDSMITH_API_KEY'
else
    echo "Deploying to cloudsmith"
    set -x
    sudo python3 -m pip install cloudsmith-cli
    cloudsmith push raw --republish --no-wait-for-sync \
        alec-leamas/opencpn-plugins-unstable $tarball
    cloudsmith push raw --republish --no-wait-for-sync \
        alec-leamas/opencpn-plugins-unstable $xml
    set +x
fi
