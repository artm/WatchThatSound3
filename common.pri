wts.pro.relative = $$replace(_PRO_FILE_,"$$PWD/","")
isEmpty(wts.top.relative) { error( " '$$wts.pro.relative' should set 'wts.top.relative' " ) }

# make sure we can refer to files by their relative paths
wts.src.root = $$PWD

# precompiled header
INCLUDEPATH += $$wts.src.root
CONFIG += precompile_header
HEADERS += $$wts.src.root/stable.h
PRECOMPILED_HEADER = $$wts.src.root/stable.h

# find component headers
INCLUDEPATH += $$wts.src.root/components

# find component libraries
LIBS += -L$$wts.top.relative/lib/ $$join(wts.components,' -l',-l)

# suppress warnings from QtSDK 4.8.1
QMAKE_LFLAGS = -w

defineReplace(parentDirname) {
  path = $$1
  path = $$dirname(path)
  path = $$dirname(path)
  return( $$basename(path) )
}

defineReplace(parentDirname_ownDirname) {
  path = $$1
  path = $$dirname(path)
  own = $$basename(path)
  path = $$dirname(path)
  parent = $$basename(path)
  return( $${parent}_$${own} )
}

CONFIG(uitools) {
  DEFINES += QT_UI_TOOLS_LIB
}

main_exe {
  # main executable is self-testing
  QT += testlib
  CONFIG += qtestlib
  wts.target.sort = "executable"
  DESTDIR = $$wts.top.relative
  TARGET = WatchThatSound
} else:component {
  TEMPLATE = lib
  DESTDIR = $$wts.top.relative/lib/
  TARGET = $$parentDirname($$_PRO_FILE_)
  wts.target.sort = "static library"
} else:test {
  QT += testlib
  CONFIG += qtestlib
  DESTDIR = $$wts.top.relative/test/
  TARGET = $$parentDirname_ownDirname($$_PRO_FILE_)
  # tests aren't app bundles, even though they may create and test GUIs
  macx:CONFIG -= app_bundle
  wts.target.sort = "test executable"
} else {
  error("Unknown component type")
}

message( "Building $$wts.target.sort '$$TARGET' from $$wts.pro.relative" )
