# WTS component:
CONFIG += component
@if "%UITOOLS%" == "true"
CONFIG += uitools
@endif
SOURCES =
HEADERS =
wts.top.relative = ../../..
include( $$wts.top.relative/common.pri )
