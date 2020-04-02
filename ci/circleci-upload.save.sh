#!/usr/bin/env bash

#
# Upload the .tar.gz and .xml artifacts to cloudsmith
#
# Builds are uploaded the either the stable or the unstable
# repo. If there is a tag pointing to current commit it goes
# to stable, otherwise to unstable.
#
# If the environment variable CLOUDSMITH_STABLE_REPO exists it is
# used as the stable repo, defaulting to the hardcoded STABLE_REPO
# value. Likewise for CLOUDSMITH_UNSTABLE_REPO/UNSTABLE_REPO
# and CLOUDSMITH_STABLE_PKG_REPO/STABLE_PKG_REPO.
#

set -xe

PKG_EXT=${CLOUDSMITH_PKG_EXT:-'deb'}

if [ -z "$CIRCLECI" ]; then
    exit 0;
fi

if [ -z "$CLOUDSMITH_API_KEY" ]; then
    echo 'Cannot deploy to cloudsmith, missing $CLOUDSMITH_API_KEY'
    exit 0
fi

BUILD_ID=${CIRCLE_BUILD_NUM:-1}
commit=$(git rev-parse --short=7 HEAD) || commit="unknown"
tag=$(git tag --contains HEAD)

xml=$(ls $HOME/project/build/*.xml)
tarball=$(ls $HOME/project/build/*.tar.gz)
tarball_basename=${tarball##*/}

source $HOME/project/build/upload-conf.sh

tarball_name=squiddio-${PKG_TARGET}-${PKG_TARGET_VERSION}-tarball
pkg=$(ls $HOME/project/build/*.${PKG_EXT})

if [ -n "$tag" ]; then
    VERSION="$tag"
    REPO="$STABLE_REPO"
    PKG_REPO="$STABLE_PKG_REPO"
else
    VERSION="${VERSION}+${BUILD_ID}.${commit}"
    REPO="$UNSTABLE_REPO"
    PKG_REPO="$UNSTABLE_REPO"
fi

sudo sed -i -e "s|@pkg_repo@|$REPO|"  $xml
sudo sed -i -e "s|@name@|$tarball_name|" $xml
sudo sed -i -e "s|@version@|$VERSION|" $xml
sudo sed -i -e "s|@filename@|$tarball_basename|" $xml

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
    --name squiddio-${PKG_TARGET}-${PKG_TARGET_VERSION}.${PKG_EXT} \
    --version ${VERSION} \
    --summary "squiddio .${PKG_EXT} installation package" \
    $PKG_REPO $pkg
