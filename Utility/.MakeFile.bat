@echo off

set /p FILENAME="FileName > "

type nul > %FILENAME%.h
type nul > %FILENAME%.cpp