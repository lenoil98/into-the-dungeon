#!/bin/sh

PKGVER=$1
SRC_PKGNAME=$2.tar.gz
PKG_PROTO_DIR=$3

if [[ "$#" -ne 3 ]] ; then
  echo "Updates PKGBUILD from prototype directory and uses source package"
  echo "to generate arch packages."
  echo "Usage: source $0 pkgver src_pkgname pkg_proto_dir"
  return 2
fi

if [[ -e "${SRC_PKGNAME}" ]] ; then
  mkdir -p _arch_packages
  cp ${SRC_PKGNAME} _arch_packages/
  cp ${PKG_PROTO_DIR}/* _arch_packages/
  cd _arch_packages
  echo "pkgver=\"${PKGVER}\"" >> PKGBUILD
  echo "source=(\"${SRC_PKGNAME}\")" >> PKGBUILD
  makepkg -g >> PKGBUILD
  makepkg -f
  echo -e "package_arch.sh: Created binary package"
  cp -v *.pkg.tar.xz ../
  if which mkaurball > /dev/null 2>&1 ; then
    mkaurball -f
    echo -e "package_arch.sh: Created AUR package"
    cp -v *.src.tar.gz ../
  else
    echo "package_arch.sh W: mkaurball missing - unable to create AUR package"
  fi
  if which namcap > /dev/null 2>&1 ; then
    namcap PKGBUILD
    namcap *.pkg.tar.xz
  fi
else
  echo "package_arch.sh E: $(pwd)/${SRC_PKGNAME}.tar.gz doesn't exist."
  echo "package_arch.sh: Build package_source first."
  return 1
fi
return 0
