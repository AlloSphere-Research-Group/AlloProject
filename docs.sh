#!/bin/sh

which doxygen || echo You need to install doxygen. type \"brew install doxygen\" in the terminal.

if [ `which apt-get 2>/dev/null` ]; then
  xdg-open AlloSystem/doc/www/doxy/html/index.html || (pushd AlloSystem/doc && doxygen Doxyfile && popd && xdg-open AlloSystem/doc/www/doxy/html/index.html)
  xdg-open Gamma/doc/html/index.html || (pushd Gamma/doc && doxygen Doxyfile && popd && xdg-open Gamma/doc/html/index.html)
  xdg-open GLV/doc/html/index.html || (pushd GLV/doc && doxygen Doxyfile && popd && xdg-open GLV/doc/html/index.html)
else
  open AlloSystem/doc/www/doxy/html/index.html || (pushd AlloSystem/doc && doxygen Doxyfile && popd && open AlloSystem/doc/www/doxy/html/index.html)
  open Gamma/doc/html/index.html || (pushd Gamma/doc && doxygen Doxyfile && popd && open Gamma/doc/html/index.html)
  open GLV/doc/html/index.html || (pushd GLV/doc && doxygen Doxyfile && popd && open GLV/doc/html/index.html)
fi
