#!/bin/bash
#the script is not completed yet

#make new dir and copy the output executable only, exclude the make, .h and .o files

#check the qmake path and assign to var to pass to "linuxdeployqt-6-x86_64.AppImage" command

#make "git init"

#copy first the edited .desktop file

#generate the appimage components
../Testers/AppsDepolyment/linuxdeployqt-6-x86_64.AppImage PlatformTester_QT-BONUS -unsupported-allow-new-glibc -qmake="/usr/bin/qmake" -appimage

#pack as an AppImage file
../Testers/AppsDepolyment/appimagetool-x86_64.AppImage ../APD_Uds_Platform_Tester-393a1c5-x86_64/
