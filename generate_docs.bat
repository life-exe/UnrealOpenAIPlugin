:: Copyright LifeEXE. All Rights Reserved.
@echo off

rmdir /s /q "Doxygen\Documentation"
doxygen "Doxygen\Doxyfile"
start "" "Documentation\html\index.html"
