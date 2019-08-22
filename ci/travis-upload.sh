#!/bin/sh
PKG="plugins"
VERSION=1.15-0.beta2
OPTS="override=1;publish=1"

set -x

if [ -z "$BINTRAY_API_KEY" ]; then
    echo 'Cannot deploy: missing $BINTRAY_API_KEY'
    exit 0
fi

cd build
tarball=$(echo squiddio*.tar.gz)
xml=$(echo squiddio-plugin*.xml)

echo "Uploading $tarball"
curl -T $tarball  -uleamas:$BINTRAY_API_KEY \
    "https://api.bintray.com/content/leamas/OpenCPN/$PKG/$VERSION/$tarball;$OPTS"
echo
echo "Uploading $xml"
curl -T $xml  -uleamas:$BINTRAY_API_KEY \
    "https://api.bintray.com/content/leamas/OpenCPN/$PKG/$VERSION/$xml;$OPTS"
echo
