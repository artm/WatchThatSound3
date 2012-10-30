TEMPLATE = subdirs
SUBDIRS = lib test
lib.file = lib/lib%ComponentName:l%.pro
test.file = test/%ComponentName:l%_test.pro
