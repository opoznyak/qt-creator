include(../../qtcreator.pri)
include(qttestrpath.pri)

isEmpty(TEMPLATE):TEMPLATE=app
QT += $$QTESTLIB
CONFIG += qt warn_on console depend_includepath testcase
CONFIG -= app_bundle

# prefix test binary with tst_
!contains(TARGET, ^tst_.*):TARGET = $$join(TARGET,,"tst_")

win32 {
    lib = $$IDE_LIBRARY_PATH;$$IDE_PLUGIN_PATH/QtProject
    lib ~= s,/,\\,g
    # the below gets added to later by testcase.prf
    check.commands = cd . & set PATH=$$lib;%PATH%& cmd /c
}
