:: Copyright LifeEXE. All Rights Reserved.
@echo off

set dirsToRemove=Intermediate Binaries Documentation Saved Build

FOR %%f in (%dirsToRemove%) do (
    rmdir /s /q %%f
)