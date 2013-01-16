#!/bin/sh

if [ "$#" -lt 1 ]
then
  echo "Usage: debCleaner.sh file.deb"
  echo "  Cleans a cpack generated file to fit better for usage with debian or"
  echo "  debian based distributions." 
  exit
fi

echo "Cleaning debian package file"
debName="${1%.*}"
echo " Base to work on: ${debName}"

if [ -d "${debName}" ]; then
  rm -r "${debName}"
fi

dpkg -x "${debName}.deb" "$debName"
dpkg -e "${debName}.deb" "$debName/DEBIAN"

chmod 755 "${debName}/usr"
chmod 755 "${debName}/usr/bin"
chmod 755 "${debName}/usr/share"
chmod 755 "${debName}/usr/share/LogWitch"
chmod 755 "${debName}/usr/share/doc"
chmod 755 "${debName}/usr/share/doc/logwitch/"

fakeroot dpkg-deb --build "${debName}"

rm -r "${debName}"
