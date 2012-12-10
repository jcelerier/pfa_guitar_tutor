#!/bin/sh
qmake GuitarTutorAPI.pro -spec ../windows_build/win32-x-g++
make
cp IGuitar.lib ../libiguitar/
