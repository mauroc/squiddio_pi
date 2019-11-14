#!/usr/bin/env bash

#
# Upload the .tar.gz and .xml artifacts to cloudsmith
#

git_head=$(git rev-parse master) || git_head="unknown"
if [ "$git_head" != "$(git rev-parse HEAD)" ]; then
    echo "Not on master branch, skipping deployment."
    exit 0
fi

if [ -z "$CLOUDSMITH_API_KEY" ]; then
    echo 'Cannot deploy to cloudsmith, missing $CLOUDSMITH_API_KEY'
    exit 0
fi

echo "Using \$CLOUDSMITH_API_KEY: ${CLOUDSMITH_API_KEY:0:4}..."

set -xe

commit=$(git rev-parse --short=7 HEAD) || commit="unknown"
now=$(date --rfc-3339=seconds) || now=$(date)

BUILD_ID=${APPVEYOR_BUILD_NUMBER:-1}
commit=$(git rev-parse --short=7 HEAD) || commit="unknown"
tag=$(git tag --contains HEAD)

xml=$(ls *.xml)
exe=$(ls *.exe)
tarball=$(ls *.tar.gz)
tarball_basename=${tarball##*/}

source ../build/upload-conf.sh

oldexe=$exe
exe=${exe/-${PKG_TARGET_VERSION}/}
exe=${exe/_${PKG_TARGET}/-pi${OCPN_API_VERSION}-${PKG_TARGET}-win32}
mv $oldexe $exe

if [ -n "$tag" ]; then
    VERSION="$tag"
    REPO="$STABLE_REPO"
    PKG_REPO="$STABLE_PKG_REPO"
else
    VERSION="${VERSION}+${BUILD_ID}.${commit}"
    REPO="$UNSTABLE_REPO"
    PKG_REPO="$UNSTABLE_REPO"
fi
tarball_name=squiddio-${PKG_TARGET}-${PKG_TARGET_VERSION}-tarball

# There is no sed available in git bash. This is nasty, but seems
# to work:
set +x
while read line; do
    line=${line/@pkg_repo@/$REPO}
    line=${line/@name@/$tarball_name}
    line=${line/@version@/$VERSION}
    line=${line/@filename@/$tarball_basename}
    echo $line
done < $xml > xml.tmp && cp xml.tmp $xml && rm xml.tmp
set -x

cloudsmith push raw --republish --no-wait-for-sync \
    --name squiddio-${PKG_TARGET}-${PKG_TARGET_VERSION}-metadata \
    --version ${VERSION} \
    --summary "squiddio opencpn plugin metadata for automatic installation" \
    $REPO $xml

cloudsmith push raw --republish --no-wait-for-sync \
    --name $tarball_name \
    --version ${VERSION} \
    --summary "squiddio opencpn plugin tarball for automatic installation" \
    $REPO $tarball

cloudsmith push raw --republish --no-wait-for-sync \
    --name squiddio-${PKG_TARGET}-${PKG_TARGET_VERSION}-installer \
    --version ${VERSION} \
    --summary "squiddio installer for manual installation" \
    $PKG_REPO $exe
