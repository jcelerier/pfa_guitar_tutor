#!/bin/sh
qmake IGuitarGUI.pro -spec ../../windows_build/win32-x-g++
echo "Attention, suppression de la librairie libIGuitar.a (pb de compatibilité)"
rm ../../libiguitar/libIGuitar.a
make
cp IGuitarGUI.exe ../../app/
