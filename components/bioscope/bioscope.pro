TEMPLATE = subdirs
SUBDIRS = lib demo
demo.depends = lib

HEADERS += Bioscope BioscopeDriver BioscopeThread
OTHER_FILES += README.md
